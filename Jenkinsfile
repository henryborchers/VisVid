pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('Build') {
      steps {
        sh '''git submodule init
git submodule update
'''
        sh '''ls -la
mkdir build'''
        dir(path: 'build') {
          sh '''cmake ..
cmake --build .'''
        }
        
      }
    }
  }
  post {
    always {
      echo 'cleaning up'
      deleteDir()
      
    }
    
  }
}