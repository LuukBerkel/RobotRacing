package application;


import application.views.resizable.ResizableCanvas;
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import javafx.scene.image.Image;

import org.jfree.fx.FXGraphics2D;

import java.awt.*;
import java.util.Objects;

public class Main extends Application {
    private ResizableCanvas canvas;
    @Override
    public void start(Stage stage) {
        BorderPane mainPane = new BorderPane();
        canvas = new ResizableCanvas(this::draw, mainPane);
        mainPane.setCenter(canvas);
        FXGraphics2D g2 = new FXGraphics2D(canvas.getGraphicsContext2D());

        AnimationTimer animationTimer = new AnimationTimer() {
            @Override
            public void handle(long now) {
                draw(g2);
                System.out.println("test");
            }
        };
        animationTimer.start();

        Scene scene = new Scene(mainPane, 600, 400);

        stage.setTitle("JavaFX + FXGraphics2D");
        stage.setScene(scene);

        Image icon = new Image(Objects.requireNonNull(getClass().getResourceAsStream("/icon.png")));
        stage.getIcons().add(icon);
        stage.show();
    }

    private void draw(Graphics2D g2) {
        g2.clearRect(0, 0, (int) canvas.getWidth(), (int) canvas.getHeight());
        if (i < 100) {
            i++;
        } else {
            i = 0;
        }

        g2.setColor(Color.BLUE);
        g2.fillRect(50+ i, 50, 200, 100);

        g2.setColor(Color.RED);
        g2.setStroke(new BasicStroke(3));
        g2.drawOval(100, 100, 200, 150);

        g2.setColor(Color.BLACK);
        g2.drawString("FXGraphics2D works!", 120, 90);

        g2.setColor(Color.ORANGE);
        g2.fillOval(-5, -5, 10, 10);
    }

    private int i = 0;
}