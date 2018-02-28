pipeline {
  agent {
    docker {
      image 'ubuntu'
    }
    
  }
  stages {
    stage('do something') {
      steps {
        echo 'hello world'
        sh 'apt-get update -y'
      }
    }
  }
}