package model;

import com.SerialPort.SerialPort;
import controller.Listener;

import javax.swing.*;

public class PortThread implements Runnable {


    private SerialPort sp;

    private boolean listen = true;
    private boolean half = false;
    private Listener controller = null;

    public PortThread(SerialPort sp, Listener controller) {
        this.sp = sp;
        this.controller = controller;
    }

    public void run() {
        byte recieved;
        int i = 0,count = 0;

        while (!Thread.currentThread().isInterrupted()) {

            try {

                recieved = sp.readByte();

                if (recieved != 0) {

                    if (recieved == Flags.flag_desar_ncon) {

                        controller.stopTimer();
                        controller.updateStatusView(true);
                        controller.enviar(false);

                        controller.resetTimer();

                        controller.getConnectionThread().setStart_time(System.nanoTime());

                    } else if (recieved == Flags.flag_half) {

                        sp.writeByte(Flags.confirm);
                        while (sp.readByte() == 0) ;

                        controller.updateHalfView();
                        this.setHalf(true);

                    } else if (recieved == Flags.flag_progress) {

                        controller.changeProgressBar();

                    } else if (recieved == Flags.flag_connection) {

                        //System.out.println("Recieving!");

                    } else if (recieved == Flags.flag_progress_reset) {

                        while ((recieved = sp.readByte()) == 0);

                        if(recieved > 15){
                            controller.maxProgressBar(2400);
                        }else{
                            controller.maxProgressBar(1200);
                        }
                        controller.resetProgressBar();

                    } else if (recieved == Flags.flag_delete_info) {

                        controller.clean_Data();

                    } else if (recieved == Flags.flag_enviar_error) {

                        controller.showErrData();

                    } else if (recieved == Flags.flag_ad) {

                        while ((recieved = sp.readByte()) == 0) ;
                        controller.setPeriod(recieved);

                    }

                    controller.getConnectionThread().setStart_time(System.nanoTime());

                }

            } catch (Exception e) {
                e.printStackTrace();
                Thread.currentThread().interrupt(); // propagate interrupt
            }

        }

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

}