pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('get info') {
      steps {
        echo 'Nope'
        sh '''cmake --version
ls'''
      }
    }
    stage('build') {
      steps {
        dir(path: 'build') {
          sh 'cmake ..'
        }
        
      }
    }
  }
}