package model;

import com.SerialPort.SerialPort;
import controller.Listener;

public class PortThread implements Runnable {


    private SerialPort sp;

    private boolean listen = false;

    private Listener controller = null;

    public PortThread(SerialPort sp, Listener controller) {
        this.sp = sp;
        this.controller = controller;
    }

    public void run() {
        byte recieved;
        boolean led;

        try {

            while (true) {

                Thread.sleep(2);

                while (listen) {

                    recieved = sp.readByte();

                    if(recieved == Flags.flag_desar_ncon){

                        controller.enviar(false);

                        controller.restart();

                        controller.updateStatusView(true);

                    }else if(recieved == Flags.flag_heart){

                        while ((recieved = sp.readByte()) == 0);

                        sp.writeByte(Flags.confirm);
                        while (sp.readByte() == 0);

                        led = (recieved == 1);

                        while ((recieved = sp.readByte()) == 0);

                        sp.writeByte(Flags.confirm);
                        while (sp.readByte() == 0);

                        controller.updateHeartView(led,(int)recieved);

                    }else if(recieved == Flags.flag_half){

                        sp.writeByte(Flags.confirm);
                        while (sp.readByte() == 0);

                        controller.updateHalfView();

                    }else if(recieved == Flags.flag_progress){

                        if(controller.isDataSet())controller.changeProgressBar();

                    }

                }

            }

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public void setEscolta(boolean escolta) {
        this.listen = escolta;
    }

    public void setPort(SerialPort port) {
        this.sp = port;
    }
}