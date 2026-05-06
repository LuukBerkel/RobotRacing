plugins {
    id("java")
    application
    id("org.openjfx.javafxplugin") version "0.1.0"
    id("org.beryx.jlink") version "4.0.0"
}

group = "application"
version = "1.0.0"

repositories {
    mavenCentral()
}

dependencies {
    implementation("org.jfree:org.jfree.fxgraphics2d:2.1.5")
    testImplementation(platform("org.junit:junit-bom:6.0.0"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

application {
    mainClass.set("application.Main")
}

tasks.withType<JavaExec>().configureEach {
    jvmArgs = listOf(
        "--add-modules", "javafx.controls,javafx.fxml",
        "--enable-native-access=javafx.graphics"
    )
}

tasks.test {
    useJUnitPlatform()
}

javafx {
    version = "26"
    modules = listOf("javafx.controls", "javafx.fxml", "javafx.swing")
}

jlink {
    imageName = "MyApp"

    options.set(listOf(
        "--strip-debug",
        "--compress", "2",
        "--no-header-files",
        "--no-man-pages"
    ))

    launcher {
        name = "MyApp"
    }

    jpackage {
        installerType = "msi"
        imageName = "MyApp"
        installerName = "MyApp"
                installerOptions = listOf(
            "--win-dir-chooser",
            "--win-menu",
            "--win-shortcut",
        )
    }
}



