package controller;

import com.SerialPort.SerialPort;
import model.Flags;
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

import static com.sun.xml.internal.messaging.saaj.packaging.mime.util.ASCIIUtility.getBytes;

public class Listener extends MouseAdapter implements ActionListener {

    private SerialPort sp;
    private PortThread portThread;
    private MainView view;
    private OptionsPanel op;
    private FunctionDisplayPanel display;
    private int last = -1;
    private ProgressBar pb;
    private Thread reciever;


    public Listener(MainView view, SerialPort sp) {
        this.view = view;
        this.op = view.getOp();
        this.display = view.getJpTop().getDisplay();

        this.sp = sp;

        refreshFunction();

        view.setPortsAndBDs(sp.getPortList(), sp.getAvailableBaudRates());
        this.portThread = new PortThread(sp, this);

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() instanceof JComboBox) {
            refreshFunction();
        }

        if (e.getActionCommand().equals("PAP")) {
            refreshFunction();
        }

        if (e.getActionCommand().equals("OFF")) {
            refreshFunction();
        }

        portThread.setEscolta(false);

        if (e.getActionCommand().equals("JB_SEND")) {

            enviar(true);

        }

        if (e.getActionCommand().equals("JB_RF")) {

            if(view.getDataMax() > 0){

                view.changeProgressBar(true);
                enviarPeticioRF();

            }else{

                view.showMessage("Error!", "No has desat cap dada al microcontrolador!\n", JOptionPane.ERROR_MESSAGE);
                enviarErrorPeticioRF();

            }


        }
        if (e.getActionCommand().equals("JB_HEART")) {

            enviarPeticioHeart();

        }

        if (e.getActionCommand().equals("JB_HALF")) {

            enviarPeticioHalf();

        }

        if (e.getActionCommand().equals("JB_CONNECT")) {

            start();

        }
        portThread.setEscolta(true);

    }

    private void enviarErrorPeticioRF(){
        try {

            sp.writeByte(Flags.flag_enviar_error);
            while (sp.readByte() == 0);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void enviarPeticioRF(){
        try {

            sp.writeByte(Flags.flag_enviar);
            while (sp.readByte() == 0);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void enviarPeticioHeart(){
        try {

            sp.writeByte(Flags.flag_heart);
            while (sp.readByte() == 0);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void enviarPeticioHalf(){

        try {

            sp.writeByte(Flags.flag_half);
            while (sp.readByte() == 0);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void refreshFunction() {
        String type = op.getType();
        if (type.equals("Tren de polsos")) {
            display.trenDePolsos(op.getPaP(), op.getOffset());
        }
        if (type.equals("Dent de serra")) {
            display.dentDeSerra(op.getPaP(), op.getOffset());
        }
        if (type.equals("Triangular")) {
            display.triangular(op.getPaP(), op.getOffset());
        }
        if (type.equals("Sinusoïdal")) {
            display.sinus(op.getPaP(), op.getOffset());
        }
        if (type.equals("Aleatori")) {
            display.random(op.getPaP(), op.getOffset());
        }
        if (type.equals("Custom")) {
            display.reset();
            op.setCustomType();
        } else {
            op.setNormalType();
        }
    }

    private boolean isComplexFunction(){
        return (op.getType().equals("Aleatori") || op.getType().equals("Custom"));
    }

    @Override
    public void mousePressed(MouseEvent event) {

        FunctionDisplayPanel p = (FunctionDisplayPanel) event.getSource();

        p.changeValue(event.getX(), event.getY());

        last = event.getX();

        op.setCustomType();
    }

    @Override
    public void mouseDragged(MouseEvent event) {

        FunctionDisplayPanel p = (FunctionDisplayPanel) event.getSource();

        int i = last;
        int curr = event.getX();
        if (last != curr) {
            for (i = last; i != curr; i += Math.signum(curr - i)) {
                p.changeValue(i, event.getY());
            }
        }
        last = curr;

        op.setCustomType();

    }

    public void enviar(boolean confirmacio) {
        portThread.setEscolta(false);

        try {

            byte[] utf8Bytes = view.getJpTop().getDisplay().getDisplayValue(view);

            view.setProgressBarStatus(utf8Bytes.length);
            view.setDataMax(utf8Bytes.length);

            if (confirmacio) {

                sp.writeByte(Flags.flag_desar);
                while (sp.readByte() == 0);

            }

            if(isComplexFunction()){

                sp.writeByte(Flags.complex);
                while (sp.readByte() == 0);

            }else{

                sp.writeByte(Flags.ncomplex);
                while (sp.readByte() == 0);

            }

            //Part de id RF
            sp.writeByte(Flags.GROUP_ID_H);
            while (sp.readByte() == 0);

            sp.writeByte(Flags.GROUP_ID_L);
            while (sp.readByte() == 0);

            int i = 0;
            for (byte value : utf8Bytes) {

                sp.writeByte(value);
                while (sp.readByte() == 0);
                view.changeProgressBar(false);
                System.out.println(" Pos "+ i);
                i++;

            }

            System.out.print(" Done!\n");

        } catch (Exception e1) {

            JOptionPane.showMessageDialog(null, "Error en enviar les dades!\n " + e1.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);

        }

    }

    public void start() {
        try {
            if(reciever == null){

                sp.openPort(view.getSp().getSelectedPort(), view.getSp().getSelectedBaud());
                portThread.setPort(sp);

                reciever = new Thread(portThread);
                reciever.start();

            }else{

                if(!reciever.isAlive()){

                    sp.openPort(view.getSp().getSelectedPort(), view.getSp().getSelectedBaud());
                    portThread.setPort(sp);
                    reciever.start();

                }

            }


        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void restart() {
        try {
            sp.closePort();
            sp.openPort(view.getSp().getSelectedPort(), view.getSp().getSelectedBaud());
            portThread.setPort(sp);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateStatusView(boolean status) {
        if (status) {
            view.setGreenStatus();
        } else {
            view.setRedStatus();
        }

    }

    public void updateHeartView(boolean led, int color){
        view.setLEDStatus(led,color);
    }

    public void updateHalfView(){
        view.setLEDStatus(false,-1);
    }

    public boolean isDataSet(){
        return (view.getDataMax() > 0);
    }

    public void changeProgressBar(){
        view.changeProgressBar(false);
    }
}
