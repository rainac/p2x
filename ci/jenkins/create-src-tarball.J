pipeline {
  parameters {
    editableChoice(name: 'branch',
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
        checkout([$class: 'GitSCM',
                  branches: [[name: "*/${params.branch}"]],
                  extensions: [[$class: 'CleanCheckout'],  [$class: 'RelativeTargetDirectory', relativeTargetDir: 'p2x'], [$class: 'LocalBranch', localBranch: '**']],
                  userRemoteConfigs: [[credentialsId: 'joy-fs', url: 'famserv.local:git/p2x']]])
      }
    }
    stage('get-version') {
      steps {
        sh 'whoami'
        sh 'groups'
        sh 'id'
        sh 'env|sort'
        dir("p2x") {
          sh 'ls -l'
          script {
              env.VERSION = sh label: 'Test', returnStdout: true, script: './vcs-version.sh'
          }
        }
      }
    }
    stage('build') {
      steps {
        dir("p2x") {
          sh './ci/scripts/create-src-tarball.sh'
        }
      }
    }
    stage('pack') {
      steps {
        dir("p2x") {
          sh 'ls -l'
          archiveArtifacts 'p2x-*.tar.gz'
        }
      }
    }
  }
}
