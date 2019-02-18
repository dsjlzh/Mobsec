import org.jetbrains.kotlin.config.KotlinCompilerVersion
// import org.jetbrains.kotlin.gradle.dsl.Coroutines


plugins {
    id("com.android.application")
    id("kotlin-android")
    id("kotlin-android-extensions")
}

android {
    buildToolsVersion("28.0.3")
    compileSdkVersion(28)

    defaultConfig {
        minSdkVersion(23)
        targetSdkVersion(28)

        applicationId = "com.gerald.mobsec"
        versionCode = 1
        versionName = "1.0"

        ndk {
            abiFilters("armeabi-v7a", "arm64-v8a")
        }
    }

//    sourceSets {
//        getByName("main") {
//            java.srcDir("src/main/kotlin")
//        }
//    }

    externalNativeBuild {
        ndkBuild.setPath("src/main/jni/Android.mk")
    }

    buildTypes {
        getByName("release") {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android.txt"), "proguard-rules.pro")
        }
    }

    dexOptions {
        javaMaxHeapSize = "4g"
    }

    lintOptions {
        isAbortOnError = false
    }
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))
    implementation("com.android.support:appcompat-v7:28.0.0")
    implementation("com.android.support.constraint:constraint-layout:1.1.3")
    implementation(kotlin("stdlib", KotlinCompilerVersion.VERSION))
    implementation(kotlin("reflect", KotlinCompilerVersion.VERSION))
    // testImplementation(kotlin("test", KotlinCompilerVersion.VERSION))
    implementation("org.jetbrains.anko:anko:0.10.8")
    // implementation("androidx.core:core-ktx:1.0.0-alpha3")
    // androidTestImplementation("com.android.support.test:runner:1.0.2")
}

kotlin {
    // coroutines are enabled anyway in 1.3 and beyond
    // experimental.coroutines = Coroutines.ENABLE
}