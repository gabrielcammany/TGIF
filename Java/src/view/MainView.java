package view;

import com.SerialPort.SerialPort;
import controller.Listener;
import javax.swing.*;
import java.awt.*;

public class MainView extends JFrame {


    private JPanel jpBottom;
    private JLabel jStatus;
    private FunctionPanel jpTop;
    private OptionsPanel op = new OptionsPanel();
    private SendPanel sp = new SendPanel();

    public MainView() {

        //Configurem els aspectes bàsics de la finestra
        this.configureWindow();

        //Afegim tots els panells
        this.addFunctionPanel();
        this.addBottomPanel();

        this.pack();
        this.setLocationRelativeTo(null);

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
        //Definim el color per defecte del label per indicar el status del usb
        jStatus = new JLabel(" ");
        setRedStatus();
        jpBottom.add(jStatus, BorderLayout.SOUTH);
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

    public void makeWindowVisible() {
        this.setVisible(true);
    }

    public OptionsPanel getOp() {
        return op;
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

    public void showMessage(String title, String msg, Icon icon) {
        JOptionPane.showMessageDialog(null, msg, title, JOptionPane.INFORMATION_MESSAGE, icon);
    }

}
