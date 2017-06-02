import org.jetbrains.kotlin.gradle.dsl.Coroutines

buildscript {
    dependencies {
        classpath("com.android.tools.build:gradle:3.0.0-alpha2")
        classpath(kotlinModule("gradle-plugin", "1.1.2-4"))
        classpath(kotlinModule("android-extensions", "1.1.2-4"))
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
}

dependencies {
    implementation(fileTree("libs").include("*.jar"))
    implementation("com.android.support:appcompat-v7:25.3.1")
    implementation("com.android.support.constraint:constraint-layout:1.0.2")
    implementation(kotlinModule("stdlib", "1.1.2-4"))
    implementation(kotlinModule("reflect", "1.1.2-4"))
    testImplementation(kotlinModule("test", "1.1.2-4"))
    implementation("org.jetbrains.anko:anko:0.10.1")
}

kotlin { // configure<org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension>
    experimental.coroutines = Coroutines.ENABLE
}

repositories {
    jcenter()
    maven { url = uri("https://maven.google.com") }
    mavenCentral()
}
