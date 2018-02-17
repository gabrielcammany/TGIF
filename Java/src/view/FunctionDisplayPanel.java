package view;

import javax.swing.*;
import java.awt.*;


public class FunctionDisplayPanel extends JPanel {

    private static final int NUM_MOSTRES = 300;
    private int[] mostres = new int[NUM_MOSTRES];
    private final Stroke large;

    public FunctionDisplayPanel() {

        this.setBackground(Color.LIGHT_GRAY);

        this.setPreferredSize(new Dimension(600, 300));

        for(int i = 0; i < NUM_MOSTRES; i++){
            mostres[i] = 0;
        }

        large = new BasicStroke(3);
    }

    @Override
    protected void paintComponent(Graphics g0) {
        super.paintComponent(g0);
        Graphics2D g = (Graphics2D) g0;
        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        g.setColor(Color.WHITE);
        for(int i = 1; i < 5; i++) {
            g0.drawLine(0, i*60, 600, i*60);
        }
        for(int i = 1; i < 6; i++) {
            g0.drawLine(i*100, 0, i*100, 300);
        }

        g.setStroke(large);
        g0.setColor(Color.BLACK);
        for(int i = 0; i < NUM_MOSTRES -1; i++){
            g0.drawLine(i*2, 300-mostres[i], (i+1)*2, 300-mostres[i+1]);
        }
    }

    public void changeValue(int x, int y) {

        if(x >= 0 && x < 600 && y > 0 && y <= 300)
            mostres[x/2] = 300 - y;

        this.repaint();
    }

    public void trenDePolsos(double pap, double off) {
        for(int i = 0; i < NUM_MOSTRES; i++){
            if((i/75) % 2 == 1){
                mostres[i] = (int) (0 + (off/5 * 300));
            }else{
                mostres[i] = (int)(((pap/5.0)*300) + (off/5 * 300));
            }
        }
        this.repaint();
    }

    public void dentDeSerra(double pap, double off) {
        for(int i = 0; i < NUM_MOSTRES; i++){
            mostres[i] = (int)(((i%75)/75.0 * (pap/5.0)*300)  + (off/5 * 300));
        }
        this.repaint();
    }

    public void triangular(double pap, double off) {
        for(int i = 0; i < NUM_MOSTRES; i++){
            if((i/75) % 2 == 1) {
                mostres[i] = (int) (((i % 75) / 75.0 * (pap/5.0)*300) + (off/5 * 300));
            }else{
                mostres[i] = (int) (((pap/5.0)*300 - ((i % 75) / 75.0 * (pap/5.0)*300)) + (off/5 * 300));
            }
        }
        this.repaint();
    }

    public void sinus(double pap, double off) {
        for(int i = 0; i < NUM_MOSTRES; i++){
            mostres[i] = (int)(((pap/5.0)*150 * Math.sin(i * 2*Math.PI/150) + (pap/5.0)*150)  + (off/5 * 300));
        }
        this.repaint();
    }

    public void random(double pap, double off) {
        for(int i = 0; i < NUM_MOSTRES; i++){
            mostres[i] = (int)((Math.random()*(pap/5.0)*300)  + (off/5 * 300));
        }
        this.repaint();
    }

    public void reset() {
        for(int i = 0; i < NUM_MOSTRES; i++){
            mostres[i] = 0;
        }
        this.repaint();
    }
}
