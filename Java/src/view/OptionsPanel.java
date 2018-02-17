package view;

import controller.Listener;

import javax.swing.*;
import java.awt.*;

public class OptionsPanel extends JPanel {

    private JTextField JTpic;
    private JTextField JToffset;
    private JComboBox JCBtipus;

    public OptionsPanel() {
        this.setLayout(new BorderLayout(10, 10));
        JPanel optionList = new JPanel(new GridLayout(3, 2, 10, 10));
        this.add(optionList, BorderLayout.NORTH);

        this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

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

}
