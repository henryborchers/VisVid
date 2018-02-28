pipeline {
  agent {
    docker {
      image 'node:7-alpine'
    }
    
  }
  stages {
    stage('do something') {
      steps {
        echo 'hello world'
        sh 'cmake --version'
      }
    }
  }
}