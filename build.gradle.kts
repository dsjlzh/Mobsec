import org.jetbrains.kotlin.gradle.dsl.Coroutines
val kotlin_version = "1.1.2-4"
val anko_version = "0.10.0"

buildscript {
    dependencies {
        classpath("com.android.tools.build:gradle:3.0.0-alpha2")
        classpath(kotlinModule("gradle-plugin"))
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

    buildTypes {
        getByName("release") {
            isMinifyEnabled = false
            proguardFiles("proguard-rules.pro")
        }
    }
}

dependencies {
    compile(fileTree("libs").include("*.jar"))
    compile("com.android.support:appcompat-v7:25.3.1")
    compile("com.android.support.constraint:constraint-layout:1.0.2")
    compile(kotlinModule("stdlib"))
    compile("org.jetbrains.anko:anko:$anko_version") {
        exclude(group = "com.google.android", module = "android")
    }
}

kotlin { // configure<org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension>
    experimental.coroutines = Coroutines.ENABLE
}

repositories {
    jcenter()
    maven { url = uri("https://maven.google.com") }
    mavenCentral()
}
