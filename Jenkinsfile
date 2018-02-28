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
mkdir build
cd build
cmake ..
cmake --build .'''
      }
    }
  }
}