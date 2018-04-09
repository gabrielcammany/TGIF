package view;

import javax.swing.*;
import java.awt.*;

public class XAxisPanel extends JPanel {

    public XAxisPanel() {
        this.setBackground(Color.gray);

        this.setPreferredSize(new Dimension(600, 30));
    }

    @Override
    protected void paintComponent(Graphics g0) {
        super.paintComponent(g0);
        Graphics2D g = (Graphics2D) g0;
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        g.setColor(Color.WHITE);
        g.drawString("0", 28, 20);
        g.drawString("100", 220, 20);
        g.drawString("200", 420, 20);
        g.drawString("300", 600, 20);

        g.drawString("mostres", 300, 25);
    }
}
