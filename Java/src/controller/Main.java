package controller;

import com.SerialPort.SerialPort;
import controller.Listener;
import view.*;
import javax.swing.*;

public class Main {

    public static void main(String[] args){

        SwingUtilities.invokeLater(() -> {

            //Inicialització llibreria SerialPort
            //Aquesta instancia de SerialPort es sobre
            //la que heu de cridar totes les funcions de la llibreria.
            //Li passem al Listener en el constructor.
            SerialPort sp = null;
            try{
                sp = new SerialPort();
            }catch(Exception e){
                e.printStackTrace();
            }


            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }

            //Vista
            MainView view = new MainView();
            //Controlador
            Listener l = new Listener(view, sp);
            //Associem vista i controlador
            view.addListeners(l);

            view.makeWindowVisible();
        });
    }
}
