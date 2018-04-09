package view;

import javax.swing.*;
import java.awt.*;

public class FunctionPanel extends JPanel {

    private FunctionDisplayPanel display = new FunctionDisplayPanel();
    private YAxisPanel yaxis = new YAxisPanel();
    private XAxisPanel xaxis = new XAxisPanel();

    public FunctionPanel() {

        this.setLayout(new BorderLayout());

        this.add(display, BorderLayout.CENTER);
        this.add(yaxis, BorderLayout.WEST);
        this.add(xaxis, BorderLayout.SOUTH);
    }

    public FunctionDisplayPanel getDisplay(){
        return display;
    }
}
