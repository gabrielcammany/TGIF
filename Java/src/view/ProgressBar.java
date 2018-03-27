package view;

import javax.swing.*;

public class ProgressBar extends JPanel {


    private int min;
    private int max;
    private int value;


    private JProgressBar jProgressBar;
    private JFrame jFrame;

    public ProgressBar(int value,int min, int max) {
        this.min = min;
        this.max = max;
        this.value = value;

        jProgressBar = new JProgressBar(new DefaultBoundedRangeModel(value,0,min,max));
        jFrame = new JFrame();
        jFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        add(jProgressBar);
        jFrame.setContentPane(this);
        jFrame.pack();
        jFrame.setLocationRelativeTo(null);
        jFrame.setVisible(true);
    }
    public void setMin(int min) {
        jProgressBar.setMinimum(min);
    }

    public void setMax(int max) {
        jProgressBar.setMinimum(max);
    }

    public void updateProgressBar(int value){
        jProgressBar.setValue(value);
    }
    public JProgressBar getjProgressBar() {
        return jProgressBar;
    }

    public void showProgressBar(){
        //jFrame.add(jProgressBar);
        jProgressBar.setValue(150);
        add(jProgressBar);
        jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        jFrame.setContentPane(this);
        jFrame.pack();
        jFrame.setVisible(true);
    }
}
