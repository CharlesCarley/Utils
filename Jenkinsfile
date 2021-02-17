pipeline {
    agent any
    stages {

        stage('Build Project') {
            steps {
                cmake arguments: '-DUtils_BUILD_TESTS=ON', installation: 'InSearchPath'
                cmakeBuild installation: 'InSearchPath'
                sh label: 'Make', returnStatus: true, script: 'make'
            }
        }
        stage('Test Project') {
            steps {
                sh returnStatus: true, script: './Tests/UtilsTests'
            }
        }
    }
}