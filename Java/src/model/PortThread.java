package model;

import com.SerialPort.SerialPort;
import controller.Listener;

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

        while (!Thread.currentThread().isInterrupted()) {

            try {

                recieved = sp.readByte();

                if (recieved != 0) {

                    if (recieved == Flags.flag_desar_ncon && listen) {

                        controller.enviar(false);

                        controller.restart();

                        controller.updateStatusView(true);
                        controller.getConnectionThread().setStart_time(System.nanoTime());

                    } else if (recieved == Flags.flag_half) {

                        sp.writeByte(Flags.confirm);
                        while (sp.readByte() == 0) ;

                        controller.updateHalfView();
                        this.setHalf(true);
                        controller.getConnectionThread().setStart_time(System.nanoTime());

                    } else if (recieved == Flags.flag_progress) {

                        if (controller.isDataSet()) controller.changeProgressBar();
                        controller.getConnectionThread().setStart_time(System.nanoTime());

                    } else if (recieved == Flags.flag_connection) {

                        //System.out.println("Recieving!");
                        controller.getConnectionThread().setStart_time(System.nanoTime());

                    } else if (recieved == Flags.flag_heart_data) {

                        while ((recieved = sp.readByte()) == 0) ;

                        controller.updateStatusView(true);

                        controller.getConnectionThread().setStart_time(System.nanoTime());

                    } else if (recieved == Flags.flag_delete_info) {

                        controller.clean_Data();

                    }

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