package view;

import controller.Listener;

import javax.swing.*;
import java.awt.*;

public class OptionsPanel extends JPanel {

    private JTextField JTpic;
    private JTextField JToffset;
    private JComboBox JCBtipus;
    private JRadioButton un;
    private JRadioButton unvintcinc;
    private ButtonGroup buttonGroup;
    private JButton reset;

    public OptionsPanel() {
        this.setLayout(new BorderLayout(20, 20));
        JPanel optionList = new JPanel(new GridLayout(7, 2, 0, 0));
        this.add(optionList, BorderLayout.NORTH);

        this.setBorder(BorderFactory.createEmptyBorder(10, 10, 0, 10));

        optionList.add(new JLabel("Tipus: "));

        String[] tipus = {"Tren de polsos", "Dent de serra", "Triangular", "Sinusoïdal", "Aleatori", "Custom"};
        JCBtipus = new JComboBox(tipus);
        JCBtipus.setAlignmentX(Component.LEFT_ALIGNMENT);
        JCBtipus.setSelectedItem("Sinusoïdal");
        optionList.add(JCBtipus);

        optionList.add(new JLabel("Tensió pic a pic: "));
        JTpic = new JTextField("5");
        optionList.add(JTpic);

        optionList.add(new JLabel("Offset: "));
        JToffset = new JTextField("0");
        optionList.add(JToffset);

        optionList.add(new JLabel("Velocitat (bps): "));
        optionList.add(new JLabel(" "));
        un = new JRadioButton("100");
        unvintcinc = new JRadioButton("125");

        buttonGroup = new ButtonGroup();
        buttonGroup.add(un);
        buttonGroup.add(unvintcinc);
        buttonGroup.setSelected(un.getModel(),true);

        reset = new JButton("Reset");
        reset.setPreferredSize(new Dimension(70, 32));

        optionList.add(un);
        optionList.add(unvintcinc);
        optionList.add(new JLabel(" "));
        optionList.add(new JLabel(" "));
        optionList.add(reset);


    }

    public void setCustomType(){
        if(JCBtipus.getSelectedItem() != "Custom") {
            JCBtipus.setSelectedItem("Custom");
        }
        JTpic.setEnabled(false);
        JToffset.setEnabled(false);
    }

    public void setNormalType(){
        JTpic.setEnabled(true);
        JToffset.setEnabled(true);
    }

    public void addActionlistener(Listener l){
        JCBtipus.addActionListener(l);
        JTpic.setActionCommand("PAP");
        JTpic.addActionListener(l);
        JToffset.setActionCommand("OFF");
        JToffset.addActionListener(l);
        un.setActionCommand("SPEED_UN");
        un.addActionListener(l);
        unvintcinc.setActionCommand("SPEED_UN_VINT");
        unvintcinc.addActionListener(l);
        reset.setActionCommand("JB_RESET");
        reset.addActionListener(l);
    }

    public String getType(){
        return (String)JCBtipus.getSelectedItem();
    }

    public double getPaP(){
        try {
            float pap = Float.parseFloat(JTpic.getText());
            if(pap >= 0 && pap <= 5){
                return pap;
            }
            return 5.0;
        }catch(NumberFormatException e){
            return 5.0;
        }
    }

    public double getOffset(){
        try {
            float off = Float.parseFloat(JToffset.getText());
            if(off >= 0 && Float.parseFloat(JTpic.getText())+off <= 5){
                return off;
            }
            return 0.0;
        }catch(NumberFormatException e){
            return 0.0;
        }
    }

    public ButtonGroup getButtonGroup() {
        return buttonGroup;
    }

    public void setButtonGroup(ButtonGroup buttonGroup) {
        this.buttonGroup = buttonGroup;
    }


    public JRadioButton getUn() {
        return un;
    }

    public void setUn(JRadioButton un) {
        this.un = un;
    }

    public JRadioButton getUnvintcinc() {
        return unvintcinc;
    }

    public void setUnvintcinc(JRadioButton unvintcinc) {
        this.unvintcinc = unvintcinc;
    }



}
