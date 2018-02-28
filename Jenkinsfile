pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }
    
  }
  stages {
    stage('build') {
      steps {
        sh '''mkdir build
git submodule init
git submodule update
'''
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