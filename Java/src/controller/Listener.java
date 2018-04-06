package controller;

import com.SerialPort.SerialPort;
import model.ConnectionThread;
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
    private ConnectionThread connectionThread;
    private MainView view;
    private OptionsPanel op;
    private FunctionDisplayPanel display;
    private int last = -1;
    private Thread reciever;
    private Thread timer;


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
        if (e.getSource() instanceof JComboBox) {
            refreshFunction();
        }

        if (e.getActionCommand().equals("PAP")) {
            refreshFunction();
        }

        if (e.getActionCommand().equals("OFF")) {
            refreshFunction();
        }

        if (e.getActionCommand().equals("JB_CONNECT")) {

            start();
            return;

        }

        if(portThread != null){

            if(!view.isConnected()){

                view.showMessage("Error!", "No estas connectat!\n", JOptionPane.ERROR_MESSAGE);

                return;
            }

            portThread.setEscolta(false);
            reciever.interrupt();
            timer.interrupt();

            //System.out.println("Recieved coinfimr:asd33");
            while (reciever.isAlive());
            while (timer.isAlive());

            view.setGreenStatus();

            switch (e.getActionCommand()){
                case "JB_HEART":
                    enviarPeticioHeart();
                    break;
                case "JB_HALF":
                    enviarPeticioHalf();
                    break;
                case "JB_SEND":
                    view.changeProgressBar(true);
                    enviar(true);
                    break;
                case "JB_RF":

                    byte recieved;

                    if ((recieved = dadesDesades()) > 0) {

                        view.changeProgressBar(true);
                        System.out.println(recieved);
                        if(recieved > 15){
                            view.setProgressBarStatus(2400);
                        }else{
                            view.setProgressBarStatus(1200);
                        }
                        enviarPeticioRF();

                    } else {

                        view.showMessage("Error!", "No has desat cap dada al microcontrolador!\n", JOptionPane.ERROR_MESSAGE);
                        enviarErrorPeticioRF();

                    }

                    break;
            }

            portThread.setEscolta(true);
            restart();

        }else{
            view.showMessage("Error!", "No estas connectat!\n", JOptionPane.ERROR_MESSAGE);
        }

    }

    private byte dadesDesades(){
        byte received;

        try {

            sp.writeByte(Flags.flag_data);
            while ((received = sp.readByte()) == 0) ;

            return received;

        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    private void enviarErrorPeticioRF() {
        try {

            sp.writeByte(Flags.flag_enviar_error);
            while (sp.readByte() == 0) ;

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void enviarPeticioRF() {
        try {

            sp.writeByte(Flags.flag_enviar);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void enviarPeticioHeart() {
        try {

            sp.writeByte(Flags.flag_heart);
            portThread.setHalf(false);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    private void enviarPeticioHalf() {

        try {

            sp.writeByte(Flags.flag_half);
            portThread.setHalf(true);

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

    private boolean isComplexFunction() {
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

        byte recieved = 0;
        try {

            byte[] utf8Bytes = view.getJpTop().getDisplay().getDisplayValue(view);

            view.setProgressBarStatus(utf8Bytes.length);
            view.setDataMax(utf8Bytes.length);

            if (confirmacio) {

                sp.writeByte(Flags.flag_desar);
                while ((recieved = sp.readByte()) == 0 || recieved != Flags.flag_desar);

            }

            //System.out.println("Recieved coinfim: " + recieved);

            if (isComplexFunction()) {

                sp.writeByte(Flags.complex);
                while ((recieved = sp.readByte()) == 0);

            } else {

                sp.writeByte(Flags.ncomplex);
                while ((recieved = sp.readByte()) == 0);

            }
            //System.out.println("Recieved complex: " + String.valueOf(recieved));

            //Part de id RF
            sp.writeByte(Flags.GROUP_ID_H);
            while ((recieved = sp.readByte()) == 0) ;
            //System.out.println("Recieved ID: " + recieved);

            sp.writeByte(Flags.GROUP_ID_L);
            while ((recieved = sp.readByte()) == 0) ;
            //System.out.println("Recieved ID2: " + recieved);

            int i = 0;
            for (byte value : utf8Bytes) {

                sp.writeByte(value);
                while ((recieved = sp.readByte()) == 0) ;

                view.changeProgressBar(false);
                view.setGreenStatus();
                //System.out.println(" Pos " + i + " " + recieved);
                i++;

            }

            this.getConnectionThread().setStart_time(System.nanoTime());

            System.out.println(" Done! Total: " + i);

        } catch (Exception e1) {

            JOptionPane.showMessageDialog(null, "Error en enviar les dades!\n " + e1.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);

        }
    }

    public void start() {
        try {
            if (reciever == null) {

                sp.openPort(view.getSp().getSelectedPort(), view.getSp().getSelectedBaud());

                portThread = new PortThread(sp, this);
                portThread.setPort(sp);

                reciever = new Thread(portThread);
                reciever.start();

                connectionThread = new ConnectionThread(this);
                timer = new Thread(connectionThread);
                timer.start();

            } else {

                if (!reciever.isAlive()) {

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
            reciever.join();
            reciever = new Thread(portThread);
            reciever.start();
            timer.join();
            timer = new Thread(connectionThread);
            timer.start();
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

    public void updateHeartView(boolean led, int color) {
        view.setLEDStatus(led, color);
    }

    public void updateHalfView() {
        view.setLEDStatus(false, -1);
    }

    public boolean isDataSet() {
        return (view.getDataMax() > 0);
    }

    public void changeProgressBar() {
        view.changeProgressBar(false);
    }

    public ConnectionThread getConnectionThread() {
        return connectionThread;
    }

    public void setConnectionThread(ConnectionThread connectionThread) {
        this.connectionThread = connectionThread;
    }

    public void clean_Data(){
        view.setDataMax(0);
    }



}
