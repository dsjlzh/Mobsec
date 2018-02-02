import org.jetbrains.kotlin.gradle.dsl.Coroutines

plugins {
    id("com.android.application") version "3.0.1"
    kotlin("android") version "1.2.21"
    kotlin("android.extensions") version "1.2.21"
}

android {
    buildToolsVersion("26.0.2")
    compileSdkVersion(25)

    defaultConfig {
        minSdkVersion(23)
        targetSdkVersion(25)

        applicationId = "com.gerald.mobsec"
        versionCode = 1
        versionName = "1.0"

        ndk {
            abiFilters("armeabi-v7a", "arm64-v8a")
        }
    }

    sourceSets {
        getByName("main") {
            java.srcDir("src/main/kotlin")
        }
    }

    externalNativeBuild {
        ndkBuild.setPath("src/main/jni/Android.mk")
    }

    buildTypes {
        getByName("release") {
            isMinifyEnabled = false
            proguardFiles("proguard-rules.pro")
        }
    }

    dexOptions {
        javaMaxHeapSize = "4g"
    }
}

dependencies {
    compile(fileTree("libs").include("*.jar"))
    compile("com.android.support:appcompat-v7:25.3.1")
    compile("com.android.support.constraint:constraint-layout:1.0.2")
    compile(kotlin("stdlib", "1.2.21"))
    compile(kotlin("reflect", "1.2.21"))
    // testCompile(kotlin("test", "1.2.21"))
    compile("org.jetbrains.anko:anko:0.10.4")
}

kotlin { // configure<org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension>
    experimental.coroutines = Coroutines.ENABLE
}

allprojects {
    repositories {
        jcenter()
        mavenCentral()
    }
}
