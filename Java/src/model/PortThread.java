package model;

import com.SerialPort.SerialPort;
import controller.Listener;

public class PortThread implements Runnable {


    private SerialPort sp = null;
    private int baudRate;
    private String port;
    private boolean escolta = true;
    private int espera = 0;
    private Listener controller = null;
    private final byte flag = (byte) (Integer.parseInt("84",16));
    private final byte connected = (byte) (Integer.parseInt("80",16));

    public PortThread(SerialPort sp, Listener controller){
        this.sp = sp;
        this.controller = controller;
    }

    public void run() {
        byte aux;

        long init;
        long end;

        while (true) {
            while (escolta) {
                try {
                    init=System.currentTimeMillis();
                    aux = sp.readByte();
                    Thread.sleep(2);

                    if (aux != 0){
                        end=System.currentTimeMillis();
                        if(1000 > (end-init)){
                            init=System.currentTimeMillis();
                            controller.updateStatusView(false);
                        }
                        System.out.print(aux);
                        controller.updateStatusView(true);
                    }
                    if (aux == flag) {
                        end=System.currentTimeMillis();
                        if(1000 > (end-init)){
                            init=System.currentTimeMillis();
                            controller.updateStatusView(false);
                        }
                        espera = 0;
                        controller.enviar(true);
                        controller.restart();
                        controller.updateStatusView(true);
                    }
                    if(aux == connected){
                        end=System.currentTimeMillis();
                        if(1000 > (end-init)){
                            init=System.currentTimeMillis();
                            controller.updateStatusView(false);
                        }
                        controller.updateStatusView(true);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            try {
                Thread.sleep(3);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void setEscolta(boolean escolta) {
        this.escolta = escolta;
    }

    public void setPort(SerialPort port) {
        this.sp = port;
    }
}