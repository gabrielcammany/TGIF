package view;

import controller.Listener;

import javax.swing.*;
import java.awt.*;

public class SendPanel extends JPanel {

    private JPanel jpConfigure;
    private JPanel jpButtons;

    private JLabel jlBaudrate = new JLabel("Baudrate: ");
    private JLabel jlport = new JLabel("Port: ");


    private JComboBox<Integer> jcbComboBaud = new JComboBox<>();
    private JComboBox<String> jcbComboPort = new JComboBox<>();

    private JButton jbSend = new JButton("Carrega funció");
    private JButton jbRF = new JButton("Enviar RF");
    private JButton jbHeart = new JButton("HeartBeat");
    private JButton jbHalf = new JButton("Mitja Intensitat");
    private JButton jbConnect = new JButton("Connecta");

    public SendPanel() {

        this.setLayout(new GridLayout(2, 2, 0, 0));
        this.setBorder(BorderFactory.createEmptyBorder(0, 10, 0, 0));

        //JPanel Configuració
        this.createConfigPanel();

        //JPanel botons comunicació PIC
        this.createButtonsPanel();

    }

    public void addActionListener(Listener l){
        jbSend.setActionCommand("JB_SEND");
        jbRF.setActionCommand("JB_RF");
        jbHeart.setActionCommand("JB_HEART");
        jbHalf.setActionCommand("JB_HALF");
        jbConnect.setActionCommand("JB_CONNECT");


        jbSend.addActionListener(l);
        jbRF.addActionListener(l);
        jbHeart.addActionListener(l);
        jbHalf.addActionListener(l);
        jbConnect.addActionListener(l);
    }

    private void createConfigPanel() {
        jpConfigure = new JPanel(new GridBagLayout());
        this.add(jpConfigure, BorderLayout.NORTH);


        GridBagConstraints gbc = new GridBagConstraints();

        jcbComboBaud.setPreferredSize(new Dimension(125, 30));
        jcbComboPort.setPreferredSize(new Dimension(125, 30));

        jpConfigure.setBorder(BorderFactory.createEmptyBorder(0, -22, 0, 0));

        gbc.gridx = 0;
        gbc.gridy = 0;
        jpConfigure.add(jlBaudrate, gbc);
        gbc.gridx = 1;
        gbc.gridy = 0;
        jpConfigure.add(jcbComboBaud, gbc);
        gbc.gridx = 0;
        gbc.gridy = 1;
        jpConfigure.add(jlport, gbc);
        gbc.gridx = 1;
        gbc.gridy = 1;
        jpConfigure.add(jcbComboPort, gbc);
        gbc.gridx = 1;
        gbc.gridy = 2;
        jpConfigure.add(new JLabel(" "), gbc);
        gbc.gridx = 0;
        gbc.gridy = 3;
        jbConnect.setPreferredSize(new Dimension(120, 42));
        jpConfigure.add(jbConnect, gbc);
    }

    private void createButtonsPanel() {
        jpButtons = new JPanel(new GridLayout(2, 2, 20, 20));
        this.add(jpButtons, BorderLayout.CENTER);
        jpButtons.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 20));

        jpButtons.add(jbSend);
        jpButtons.add(jbRF);
        jpButtons.add(jbHeart);
        jpButtons.add(jbHalf);

    }

    public void setPortsAndBDs(String[] ports, int[] bds){

        setPortsList(ports);
        setBaudRateList(bds);
    }

    private void setPortsList(String [] lPorts){
        jcbComboPort.removeAllItems();
        for(String item : lPorts){
            jcbComboPort.addItem(item);
        }
    }

    private void setBaudRateList(int [] lBaudRates){
        jcbComboBaud.removeAllItems();
        for(int item : lBaudRates){
            jcbComboBaud.addItem(item);
        }
    }

    public Integer getSelectedBaud() {
        return (Integer) jcbComboBaud.getSelectedItem();
    }


    public String getSelectedPort() {
        return (String) jcbComboPort.getSelectedItem();
    }

}
