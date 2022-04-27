pipeline {
  parameters {
    editableChoice(name: 'branch',
           choices: ['debian%2Fexperimental'],
           description: 'Git branch')
    choice(name: 'container',
           choices: ['debian-stable', 'debian-testing', 'debian-unstable', 'debian-bullseye'],
           description: 'Container Image')
    choice(name: 'DIST',
           choices: ['bullseye', 'buster', 'stable', 'testing', 'unstable'],
           description: 'Debian Dist')
    choice(name: 'ARCH',
           choices: ['amd64', 'i368'],
           description: 'Debian Arch')
  }
  agent {
    dockerfile {
      filename 'Dockerfile.build'
      dir "docker/${params.container}"
      label "builder"
      additionalBuildArgs  ''
      args '--user=root'
    }
  }
  stages {
    stage('checkout') {
      steps {
        copyArtifacts(filter: "${params.DIST}/p2x_*.deb",
                      fingerprintArtifacts: true,
                      flatten: true,
                      parameters: "ARCH=$ARCH,DIST=$DIST",
                      projectName: "p2x-debian-build-package/${params.branch}",
                      selector: lastSuccessful(),
                      target: 'pkg')
        sh 'ls -l'
        sh 'ls -l pkg'
      }
    }
    stage('get-version') {
      steps {
        sh 'whoami'
        sh 'groups'
        sh 'id'
        sh 'env|sort'
      }
    }
    stage('build') {
      steps {
        sh "dpkg -i pkg/*.deb"

        sh "autoreconf -i"
        sh "./configure"

        dir("tests/sh") {
          sh "make check P2X=p2x"
        }
      }
    }
    stage('pack') {
      steps {
        sh "sudo dpkg -l p2x | tee p2x-installed.txt"
        archiveArtifacts "p2x-installed.txt"
        archiveArtifacts "tests/sh/log_*.txt"
      }
    }
  }
}
