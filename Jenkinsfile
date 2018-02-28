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
        sh '''git submodule update --recursive --remote -f
mkdir build
cd build
cmake ..
cmake --build .'''
      }
    }
  }
}