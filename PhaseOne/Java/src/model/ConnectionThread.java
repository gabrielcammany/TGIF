package model;

import controller.Listener;

public class ConnectionThread  implements Runnable  {

    private long start_time;
    private Listener controller;


    public ConnectionThread(Listener controller){

        this.controller = controller;
    }

    @Override
    public void run() {

        while (!Thread.currentThread().isInterrupted()) {
            try {

                Thread.sleep(300);

                if(((System.nanoTime() - start_time) / 1e6) > 300){
                    controller.updateStatusView(false);
                }else{
                    controller.updateStatusView(true);
                }

            } catch (Exception e) {
                Thread.currentThread().interrupt(); // propagate interrupt
            }
        }
    }

    public long getStart_time() {
        return start_time;
    }

    public void setStart_time(long start_time) {
        this.start_time = start_time;
    }

}
