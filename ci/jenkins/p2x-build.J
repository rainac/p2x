pipeline {
  parameters {
    string(name: 'PERSONAL_KEY', defaultValue: '75f375f3', description: 'Unique data')
    choice(name: 'ARCH', choices: ['x64', 'i686'], description: '')
    choice(name: 'CONFIG', choices: ['Release', 'Debug'], description: '')
    choice(name: 'MARCH', choices: ['x86-64', 'sandybridge', 'core2', 'amdfam10'], description: '')
    string(name: 'CPPFLAGS', defaultValue: '', description: '')
    string(name: 'CXXFLAGS', defaultValue: '', description: '')
    string(name: 'STRIPFLAGS', defaultValue: '', description: '')
    string(name: 'LDFLAGS', defaultValue: '', description: '')
    choice(name: 'container',
           choices: ['debian-stable', 'debian-testing', 'debian-unstable'],
           description: 'Container Image')
  }
  agent {
    dockerfile {
      filename 'Dockerfile'
      dir "docker/${params.container}"
      label "builder"
      additionalBuildArgs  ''
      args '--user=root'
    }
  }
  stages {
    stage('checkout') {
      steps {
        sh 'ls -l'
        sh 'env | sort'
      }
    }
    stage('get-version') {
      steps {
        sh 'whoami'
        sh 'groups'
        sh 'id'
        sh 'env|sort'
        sh 'ls -l'
        sh './autogen.sh'
        script {
            env.VERSION = sh label: 'Test', returnStdout: true, script: './vcs-version.sh'
            env.CPPFLAGS = "-DNDEBUG"
            env.CXXFLAGS = "-O3 -Wall -Wextra"
            env.CFLAGS = "-O3 -Wall -Wextra"
        }
      }
    }
    stage('build') {
      environment {
        LDFLAGS = "${params.LDFLAGS}"
        STRIPFLAGS = "-s -R .comment -R '.note*' ${params.STRIPFLAGS}"
        CXXFLAGS = "-O3 -Wall -Wextra -fno-exceptions -fno-rtti -march=${params.MARCH} ${params.CXXFLAGS}"
        CPPFLAGS = '-DNDEBUG' + " -DCOMPILE_MARCH='\"${params.MARCH}\"' ${params.CPPFLAGS}"
      }
      steps {
          sh './configure'
          sh 'make install'
      }
    }
    stage('pack') {
      steps {
        sh 'ls -l'
        sh 'ls -l /opt/opx-server/'
        sh 'cd /opt/opx-server && tar -czf opx-server-$BUILD_VERSION.tar.gz opx-server-$VERSION'
        sh 'cp /opt/opx-server/*.tar.gz opx-src'
        dir('opx-src') {
            sh './mk-tarball-self.sh'
            sh 'ls -l'
            sh 'mv runtime/R/adr_*.tar.gz .'
            archiveArtifacts 'opx-server-*.tar.gz'
            archiveArtifacts 'opx-*.tar.gz'
            archiveArtifacts 'adr_*.tar.gz'
        }
        sh 'rm -rf opx-src'
        sh 'rm -rf /opt/opx-server'
      }
    }
  }
}
