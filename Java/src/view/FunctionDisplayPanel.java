package view;

import javax.swing.*;
import java.awt.*;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;

import static java.lang.Math.floor;


public class FunctionDisplayPanel extends JPanel {

    //DEFAULT --> private static final int NUM_MOSTRES = 300;
    private static final int NUM_MOSTRES = 300;
    private int[] mostres = new int[NUM_MOSTRES];
    private byte[] BMostres;



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

    public byte[] getFullMostresScaled() {
        byte []mostres_scaled = new byte[NUM_MOSTRES];
        int i = 0;

        for(int value:mostres)
            mostres_scaled[i++]=(byte)floor(256 * value / (300+ 1));

        return mostres_scaled;
    }

    public byte[] getPartMostresScaled() {
        byte []mostres_scaled = new byte[150];
        int i = 0;

        for(int value: Arrays.copyOf(mostres,150))
            mostres_scaled[i++]=(byte)floor(256 * value / (300+ 1));

        return mostres_scaled;
    }
    public int[] getFullMostres() {
        return mostres;
    }
    public int[] getPartMostres() {
        return Arrays.copyOf(mostres,150);
    }

    public byte[] getDisplayValue(MainView view) {
        switch (view.getOp().getType().toLowerCase()){
            case "sinusoïdal": case "tren de polsos": case"triangular": case"dent de serra":
                return getPartMostresScaled();
            case "aleatori": case"custom":
                return getFullMostresScaled();
        }
        return null;
    }

    public byte[] tobyte(int[]data){
        ByteBuffer byteBuffer = ByteBuffer.allocate(data.length * 4);
        IntBuffer intBuffer = byteBuffer.asIntBuffer();
        intBuffer.put(data);
        BMostres = byteBuffer.array();
        return BMostres;

    }
}
