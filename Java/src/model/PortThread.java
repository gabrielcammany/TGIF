package model;

import com.SerialPort.SerialPort;
import controller.Listener;

public class PortThread implements Runnable {


    private SerialPort sp;

    private boolean listen = true;
    private boolean recieving = false;
    private boolean connected = false;
    private boolean half = false;
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

                Thread.sleep(100);
                //System.out.println("Recieving!5");


                while (listen) {

                    System.out.println("Recieving!1");
                    this.recieving = true;
                    System.out.println("Recieving!2");

                    recieved = sp.readByte();
                    System.out.println("Recieving!4"+ listen);

                    if(listen && recieved != 0){

                        System.out.println("Recieving!3");

                        if(recieved == Flags.flag_desar_ncon && listen){

                            controller.enviar(false);

                            controller.restart();

                            controller.updateStatusView(true);
                            controller.getConnectionThread().setStart_time(System.nanoTime());

                        }else if(recieved == Flags.flag_half){

                            sp.writeByte(Flags.confirm);
                            while (sp.readByte() == 0);

                            controller.updateHalfView();
                            this.setHalf(true);
                            controller.getConnectionThread().setStart_time(System.nanoTime());

                        }else if(recieved == Flags.flag_progress){

                            if(controller.isDataSet())controller.changeProgressBar();
                            controller.getConnectionThread().setStart_time(System.nanoTime());

                        }else if(recieved == Flags.flag_connection){

                            System.out.println("Recieving!");
                            controller.getConnectionThread().setStart_time(System.nanoTime());

                        }

                    }

                    this.recieving = false;

                }

            }

        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println("Done");

    }


    public boolean isRecieving() {
        return recieving;
    }

    public void setRecieving(boolean recieving) {
        this.recieving = recieving;
    }

    public void setEscolta(boolean escolta) {
        this.listen = escolta;
    }

    public boolean isHalf() {
        return half;
    }

    public void setHalf(boolean half) {
        this.half = half;
    }
    public void setPort(SerialPort port) {
        this.sp = port;
    }

    public boolean isConnected() {
        return connected;
    }

    public void setConnected(boolean connected) {
        this.connected = connected;
    }
}