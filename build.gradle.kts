import org.jetbrains.kotlin.gradle.dsl.Coroutines
/*
buildscript {
    dependencies {
        classpath("com.android.tools.build:gradle:3.0.0")
        classpath(kotlin("gradle-plugin", "1.1.51"))
        classpath(kotlin("android-extensions", "1.1.51"))
    }
    repositories {
        maven { url = uri("https://maven.google.com") }
        jcenter()
    }
}

apply {
    plugin("com.android.application")
    plugin("kotlin-android")
    plugin("kotlin-android-extensions")
}
*/
plugins {
    id("com.android.application") version "2.3.3"
    kotlin("android") version "1.1.51"
    kotlin("android.extensions") version "1.1.51"
}

android {
    buildToolsVersion("25.0.3")
    compileSdkVersion(25)

    defaultConfig {
        minSdkVersion(23)
        targetSdkVersion(25)

        applicationId = "com.gerald.mobsec"
        versionCode = 1
        versionName = "1.0"
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
    compile(kotlin("stdlib", "1.1.51"))
    compile(kotlin("reflect", "1.1.51"))
    testCompile(kotlin("test", "1.1.51"))
    compile("org.jetbrains.anko:anko:0.10.2")
}

kotlin { // configure<org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension>
    experimental.coroutines = Coroutines.ENABLE
}

allprojects {
    repositories {
        jcenter()
        maven { url = uri("https://maven.google.com") }
        mavenCentral()
    }
}
