pipeline {
  parameters {
    choice(name: 'branch',
           choices: ['master', 'devel', 'debian/experimental'],
           description: 'Git branch')
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
        sh "git checkout ${params.branch}"
      }
    }
    stage('get-version') {
      steps {
        sh 'whoami'
        sh 'groups'
        sh 'id'
        sh 'env|sort'
        sh 'ls -l'
        script {
            env.VERSION = sh label: 'Test', returnStdout: true, script: './vcs-version.sh'
        }
      }
    }
    stage('build') {
      steps {
          sh './ci/scripts/create-src-tarball.sh'
      }
    }
    stage('pack') {
      steps {
        sh 'ls -l'
        archiveArtifacts 'p2x-*.tar.gz'
      }
    }
  }
}
