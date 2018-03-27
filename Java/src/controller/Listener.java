package controller;

import com.SerialPort.SerialPort;
import view.FunctionDisplayPanel;
import view.MainView;
import view.OptionsPanel;
import view.ProgressBar;
import model.PortThread;

import javax.print.DocFlavor;
import javax.swing.*;
import java.awt.event.*;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class Listener extends MouseAdapter implements ActionListener {
    private SerialPort sp = null;
    private PortThread portThread = null;
    private MainView view;
    private OptionsPanel op;
    private FunctionDisplayPanel display;
    private int last = -1;
    private ProgressBar pb;

    private final byte end_byte = (byte) (Integer.parseInt("88",16));
    private final byte start_byte_rf = (byte) (Integer.parseInt("40",16));
    private final byte end_byte_rf = (byte) (Integer.parseInt("00",16));
    private final byte flag_desar_msg = (byte) (Integer.parseInt("81",16));
    private final byte flag_enviar_rf_msg = (byte) (Integer.parseInt("82",16));
    private final byte flag = (byte) (Integer.parseInt("84",16));
    private final byte primer_byte_part_ID = (byte) (Integer.parseInt("37",16));
    private final byte segon_byte_part_ID = (byte) (Integer.parseInt("37",16));
    private final byte tercer_byte_part_ID = (byte) (Integer.parseInt("39",16));
    private final byte espai_byte_part_ID = (byte) (Integer.parseInt("20",16));


    public Listener(MainView view, SerialPort sp) {
        this.view = view;
        this.op = view.getOp();
        this.display = view.getJpTop().getDisplay();

        this.sp = sp;

        refreshFunction();

        view.setPortsAndBDs(sp.getPortList(), sp.getAvailableBaudRates());
        this.portThread = new PortThread(sp,this);
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
            pb = new ProgressBar(0,0,300);
        }

        if(e.getActionCommand().equals("JB_RF")){
            //JCBtipus.getSelectedItem()
            System.out.println("value of R1 --> "+view.getOp().getType());
            System.out.println("value of R2 --> "+Arrays.toString(view.getJpTop().getDisplay().getDisplayValue(view)));
            pb.updateProgressBar(150);
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


    public void enviar(boolean confirmacio){
        try {

        byte resposta;

        byte[] utf8Bytes = view.getJpTop().getDisplay().tobyte(view.getJpTop().getDisplay().getDisplayValue(view));
        if(confirmacio){
            sp.writeByte(flag);
        }else{
            sp.writeByte(flag_desar_msg);
            resposta = sp.readByte();
            while(resposta == 0){
                resposta = sp.readByte();
            }
        }
        //Part de id RF
        sp.writeByte(start_byte_rf);
        resposta = sp.readByte();
        while(resposta == 0){
            resposta = sp.readByte();
        }
        sp.writeByte(primer_byte_part_ID);
        resposta = sp.readByte();
        while(resposta == 0){
            resposta = sp.readByte();
        }
        sp.writeByte(segon_byte_part_ID);
        resposta = sp.readByte();
        while(resposta == 0){
            resposta = sp.readByte();
        }
        sp.writeByte(tercer_byte_part_ID);
        resposta = sp.readByte();
        while(resposta == 0){
            resposta = sp.readByte();
        }

        for (byte value:utf8Bytes) {
            sp.writeByte(value);
            resposta = sp.readByte();
            while(resposta == 0){
                resposta = sp.readByte();
            }
        }
        sp.writeByte(end_byte_rf);
        resposta = sp.readByte();
        while(resposta == 0){
            resposta = sp.readByte();
        }
        System.out.print(" Done Send!\n");
        sp.writeByte(end_byte);
        System.out.print(" Done!\n");
        } catch (Exception e1) {
            JOptionPane.showMessageDialog(null, "Error en enviar les dades!\n "+ e1.getMessage(), "Error",JOptionPane.ERROR_MESSAGE);

        }

    }

    public void start(){
        try {
            sp.openPort(view.getSp().getSelectedPort(),view.getSp().getSelectedBaud());
            portThread.setPort(sp);
            portThread.run();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void restart() {
        try {
            sp.closePort();
            sp.openPort(view.getSp().getSelectedPort(),view.getSp().getSelectedBaud());
            portThread.setPort(sp);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateStatusView(boolean status){
        if(status){
            view.setGreenStatus();
        }else{
            view.setRedStatus();
        }

    }
}
