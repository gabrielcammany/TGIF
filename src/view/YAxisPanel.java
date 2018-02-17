package view;

import javax.swing.*;
import java.awt.*;

public class YAxisPanel extends JPanel {

    public YAxisPanel() {
        this.setBackground(Color.gray);

        this.setPreferredSize(new Dimension(30, 300));
    }

    @Override
    protected void paintComponent(Graphics g0) {
        super.paintComponent(g0);
        Graphics2D g = (Graphics2D) g0;
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        g.setColor(Color.WHITE);
        g.drawString("0", 15, 300);
        g.drawString("1", 15, 245);
        g.drawString("2", 15, 185);
        g.drawString("3", 15, 125);
        g.drawString("4", 15, 65);
        g.drawString("5", 15, 15);

        g.drawString("V", 5, 155);
    }
}
