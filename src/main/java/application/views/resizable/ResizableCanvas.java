package application.views.resizable;

import application.views.Drawable;
import javafx.scene.layout.BorderPane;
import javafx.scene.canvas.Canvas;

import javafx.scene.canvas.GraphicsContext;
import org.jfree.fx.FXGraphics2D;

public class ResizableCanvas extends Canvas {
    private final Drawable observer;
    private final FXGraphics2D g2d;

    public ResizableCanvas(Drawable observer, BorderPane borderPane) throws IllegalArgumentException {
        this.observer = observer;
        this.g2d = new FXGraphics2D(this.getGraphicsContext2D());

        borderPane.widthProperty().addListener((_) -> this.resize(borderPane.getWidth(), borderPane.getHeight()));
        borderPane.heightProperty().addListener((_) -> this.resize(borderPane.getWidth(), borderPane.getHeight()));
        this.heightProperty().addListener((_) -> this.redraw());
        this.widthProperty().addListener((_) -> this.redraw());
    }

    public boolean isResizable() {
        return true;
    }

    public void resize(double width, double height) {
        super.setWidth(width);
        super.setHeight(height);
        if (width > (double)0.0F && height > (double)0.0F) {
            this.redraw();
        }
    }

    private void redraw() {
        int width = (int)this.getWidth();
        int height = (int)this.getHeight();
        GraphicsContext gc = this.getGraphicsContext2D();
        gc.clearRect(0.0F, 0.0F, width, height);
        this.observer.draw(this.g2d);
    }
}