pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
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