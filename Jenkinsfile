pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('build') {
      steps {
        sh '''git submodule init
git submodule update
cd build
cmake ..
cmake --build .'''
      }
    }
  }
  post {
    always {
      echo 'cleaning up'
      
    }
    
  }
}