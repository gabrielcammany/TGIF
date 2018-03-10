package controller;

import com.SerialPort.SerialPort;
import view.FunctionDisplayPanel;
import view.MainView;
import view.OptionsPanel;
import javax.swing.*;
import java.awt.event.*;
import java.util.Arrays;

public class Listener extends MouseAdapter implements ActionListener {

    private MainView view;
    private OptionsPanel op;
    private FunctionDisplayPanel display;
    private int last = -1;

    private SerialPort sp;

    public Listener(MainView view, SerialPort sp) {
        this.view = view;
        this.op = view.getOp();
        this.display = view.getJpTop().getDisplay();

        this.sp = sp;

        refreshFunction();

        view.setPortsAndBDs(sp.getPortList(), sp.getAvailableBaudRates());
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource() instanceof JComboBox){
            refreshFunction();
        }

        if(e.getActionCommand().equals("PAP")){
            refreshFunction();
        }

        if(e.getActionCommand().equals("OFF")){
            refreshFunction();
        }

        if(e.getActionCommand().equals("JB_SEND")){
            System.out.println("value of L --> "+Arrays.toString(view.getJpTop().getDisplay().getFullMostresScaled()));
            System.out.println("value of L2 --> "+ Arrays.toString(Arrays.copyOf(view.getJpTop().getDisplay().getFullMostresScaled(),150)));
            //view.showMessage("Configura'm!", "Implementa aquesta funció a:\nController.Listener -> ActionPerfomed", null);

        }

        if(e.getActionCommand().equals("JB_RF")){
            //JCBtipus.getSelectedItem()
            System.out.println("value of R1 --> "+view.getOp().getType());
            System.out.println("value of R2 --> "+Arrays.toString(view.getJpTop().getDisplay().getDisplayValue(view)));
            //TODO INFORMAR PLACA QUE ENVII A ESTACIO REMOTA
            //view.showMessage("Configura'm!", "Implementa l'enviament de RF a:\nController.Listener -> ActionPerfomed", null);

        }
    }

    private void refreshFunction(){
        String type = op.getType();
        if(type.equals("Tren de polsos")){
            display.trenDePolsos(op.getPaP(), op.getOffset());
        }
        if(type.equals("Dent de serra")){
            display.dentDeSerra(op.getPaP(), op.getOffset());
        }
        if(type.equals("Triangular")){
            display.triangular(op.getPaP(), op.getOffset());
        }
        if(type.equals("Sinusoïdal")){
            display.sinus(op.getPaP(), op.getOffset());
        }
        if(type.equals("Aleatori")){
            display.random(op.getPaP(), op.getOffset());
        }
        if(type.equals("Custom")){
            display.reset();
            op.setCustomType();
        }else{
            op.setNormalType();
        }
    }

    @Override
    public void mousePressed(MouseEvent event) {

        FunctionDisplayPanel p = (FunctionDisplayPanel)event.getSource();

        p.changeValue(event.getX(), event.getY());

        last = event.getX();

        op.setCustomType();
    }

    @Override
    public void mouseDragged(MouseEvent event) {

        FunctionDisplayPanel p = (FunctionDisplayPanel)event.getSource();

        int i = last;
        int curr = event.getX();
        if(last != curr) {
            for(i = last; i != curr; i+=Math.signum(curr-i)) {
                p.changeValue(i, event.getY());
            }
        }
        last = curr;

        op.setCustomType();

    }

}
