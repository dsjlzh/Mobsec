/*
 * Copyright 2016 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.gradle.script.lang.kotlin

import org.gradle.api.Project
import org.gradle.script.lang.kotlin.*

/**
 * Retrieves or configures the [ext][org.gradle.api.plugins.ExtraPropertiesExtension] project extension.
 */
fun Project.`ext`(configure: org.gradle.api.plugins.ExtraPropertiesExtension.() -> Unit = {}) =
    extensions.getByName<org.gradle.api.plugins.ExtraPropertiesExtension>("ext").apply { configure() }

/**
 * Retrieves or configures the [defaultArtifacts][org.gradle.api.internal.plugins.DefaultArtifactPublicationSet] project extension.
 */
fun Project.`defaultArtifacts`(configure: org.gradle.api.internal.plugins.DefaultArtifactPublicationSet.() -> Unit = {}) =
    extensions.getByName<org.gradle.api.internal.plugins.DefaultArtifactPublicationSet>("defaultArtifacts").apply { configure() }

/**
 * Retrieves or configures the [reporting][org.gradle.api.reporting.ReportingExtension] project extension.
 */
fun Project.`reporting`(configure: org.gradle.api.reporting.ReportingExtension.() -> Unit = {}) =
    extensions.getByName<org.gradle.api.reporting.ReportingExtension>("reporting").apply { configure() }

/**
 * Retrieves or configures the [android][com.android.build.gradle.AppExtension] project extension.
 */
fun Project.`android`(configure: com.android.build.gradle.AppExtension.() -> Unit = {}) =
    extensions.getByName<com.android.build.gradle.AppExtension>("android").apply { configure() }

/**
 * Retrieves or configures the [kotlin][org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension] project extension.
 */
fun Project.`kotlin`(configure: org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension.() -> Unit = {}) =
    extensions.getByName<org.jetbrains.kotlin.gradle.dsl.KotlinProjectExtension>("kotlin").apply { configure() }

/**
 * Retrieves or configures the [kapt][org.jetbrains.kotlin.gradle.plugin.KaptExtension] project extension.
 */
fun Project.`kapt`(configure: org.jetbrains.kotlin.gradle.plugin.KaptExtension.() -> Unit = {}) =
    extensions.getByName<org.jetbrains.kotlin.gradle.plugin.KaptExtension>("kapt").apply { configure() }

/**
 * Retrieves or configures the [base][org.gradle.api.plugins.BasePluginConvention] project convention.
 */
fun Project.`base`(configure: org.gradle.api.plugins.BasePluginConvention.() -> Unit = {}) =
    convention.getPluginByName<org.gradle.api.plugins.BasePluginConvention>("base").apply { configure() }

/**
 * Retrieves or configures the [java][org.gradle.api.plugins.JavaPluginConvention] project convention.
 */
fun Project.`java`(configure: org.gradle.api.plugins.JavaPluginConvention.() -> Unit = {}) =
    convention.getPluginByName<org.gradle.api.plugins.JavaPluginConvention>("java").apply { configure() }

