package view;

import controller.Listener;
import javax.swing.*;
import java.awt.*;

public class MainView extends JFrame {


    private JPanel jpBottom;
    private JLabel jStatus;
    private JLabel jStatusZero;
    private JLabel jStatusOne;
    private FunctionPanel jpTop;
    private OptionsPanel op = new OptionsPanel();
    private SendPanel sp = new SendPanel();
    private JProgressBar jProgressBar;


    private int dataMax;

    public MainView() {

        //Configurem els aspectes bàsics de la finestra
        this.configureWindow();

        //Afegim tots els panells
        this.addFunctionPanel();
        this.addBottomPanel();

        this.pack();
        this.setLocationRelativeTo(null);
        dataMax = 0;

    }

    private void configureWindow() {
        this.setTitle("TeleGenerador");
        this.setResizable(false);
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        this.setLayout(new BorderLayout());

    }

    private void addFunctionPanel() {
        jpTop = new FunctionPanel();
        this.add(jpTop, BorderLayout.CENTER);
    }

    private void addBottomPanel() {
        jpBottom = new JPanel(new BorderLayout());
        this.add(jpBottom, BorderLayout.SOUTH);
        op = new OptionsPanel();
        jpBottom.add(op, BorderLayout.WEST);

        sp = new SendPanel();
        jpBottom.add(sp, BorderLayout.CENTER);

        configProgressBar();
        jpBottom.add(jProgressBar,BorderLayout.NORTH);

        JPanel sliderPanel = new JPanel((new GridLayout(0, 1)));
        JPanel status = new JPanel((new GridLayout(0, 2)));
        configLEDS();

        JLabel title = new JLabel("LEDs Status", SwingConstants.CENTER);

        sliderPanel.add(title);
        title.setFont(new Font("Arial", Font.BOLD, 14));
        sliderPanel.add(new JSeparator());
        status.add(jStatusZero);
        status.add(jStatusOne);
        sliderPanel.add(status);
        sliderPanel.add(Box.createVerticalStrut(2));
        sliderPanel.add(jStatus);

        sliderPanel.revalidate();

        jpBottom.add(sliderPanel, BorderLayout.PAGE_END);
        //Definim el color per defecte del label per indicar el status del usb


    }

    private void configLEDS(){

        jStatus = new JLabel("Not connected", SwingConstants.CENTER);
        setRedStatus();
        jStatusOne = new JLabel("Not connected", SwingConstants.CENTER);
        jStatusZero = new JLabel("Not connected", SwingConstants.CENTER);
    }

    public void  setGreenStatus(){
        this.jStatus.setBackground(new Color(0,150,0));
        this.jStatus.setOpaque(true);
        repaint();
    }

    public void  setRedStatus(){
        this.jStatus.setBackground(new Color(150,0,0));
        this.jStatus.setOpaque(true);
        repaint();
    }

    public void setProgressBarStatus(int max){
        this.jProgressBar.setMinimum(0);
        this.jProgressBar.setMaximum(max);
    }

    public void changeProgressBar(boolean reset){
        if(!reset){
            this.jProgressBar.setValue(this.jProgressBar.getValue() + 1);
        }else{
            this.jProgressBar.setValue(0);
        }
    }

    public void setLEDStatus(boolean led,int value){

        if(value > 0){
            if(led){

                this.jStatusZero.setVisible(false);
                this.jStatusOne.setVisible(true);
                this.jStatusOne.setBackground(new Color(0,0,255 - value));
                this.jStatusOne.setOpaque(true);

            }else{

                this.jStatusZero.setVisible(true);
                this.jStatusOne.setVisible(false);
                this.jStatusZero.setBackground(new Color(0,255 - value,0));
                this.jStatusZero.setOpaque(true);

            }
        }else{

            this.jStatusOne.setVisible(true);
            this.jStatusZero.setVisible(true);

            this.jStatusZero.setBackground(new Color(0,150,0));
            this.jStatusZero.setBackground(new Color(0,0,150));

            this.jStatusZero.setOpaque(true);

        }
        repaint();

    }

    private void configProgressBar(){

        jProgressBar = new JProgressBar();
        jProgressBar.setMinimum(0);
        jProgressBar.setMaximum(300);
        jProgressBar.setValue(0);
        jProgressBar.setFont(new java.awt.Font("Tahoma", 0, 14)); // NOI18N

    }

    public void makeWindowVisible() {
        this.setVisible(true);
    }

    public OptionsPanel getOp() {
        return op;
    }

    public SendPanel getSp() {
        return sp;
    }

    public FunctionPanel getJpTop() {
        return jpTop;
    }

    public void setPortsAndBDs(String[] ports, int[] bds){
        sp.setPortsAndBDs(ports, bds);
    }

    public void addListeners(Listener l) {
        jpTop.getDisplay().addMouseMotionListener(l);
        jpTop.getDisplay().addMouseListener(l);
        op.addActionlistener(l);
        sp.addActionListener(l);
    }

    public void showMessage(String title, String msg, int messageType) {
        JOptionPane.showMessageDialog(null, msg, title, messageType, null);
    }

    public int getDataMax() {
        return dataMax;
    }

    public void setDataMax(int dataMax) {
        this.dataMax = dataMax;
    }

}
