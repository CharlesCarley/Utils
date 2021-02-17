pipeline {
    agent any
    stages {
        stage('Build Project') {
            steps {
                sh label: 'CMake', returnStatus: 0, script: '''
                    mkdir Build
                    cd Build
                    cmake .. -DUtils_BUILD_TESTS=ON
                    make
                '''
            }
        }
        stage('Test Project') {
            steps {
                sh returnStatus: 0, script: './Tests/UtilsTests'
            }
        }
    }
}