import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.ArrayList;
import javax.swing.DefaultListModel;

public class UDPThread extends Thread {
	DatagramSocket socket;
	@Override
	public void run()
	{
		try {
			socket = new DatagramSocket(4000);
		} catch (SocketException e) {e.printStackTrace();}
		System.out.println("start UDP");
                String dataString;
                long preTime = 0;
		while(true)
		{
                        System.out.println("udp is running");
			byte buffer[] = new byte[20];
			DatagramPacket packet = new DatagramPacket(buffer,20);
			
                        try {
                                System.out.println("recive data");
				socket.receive(packet);
                                System.out.println("recive success");
			} catch (IOException e) {e.printStackTrace();}
			buffer = packet.getData();
                        System.out.println("udpdata recive");
                        Packet recivePacket = new Packet(buffer);
			dataString = new String(recivePacket.data);
                        System.out.println( "recived Packet : " + dataString+ " " + recivePacket.flag + " " +recivePacket.sen_id +" "+ recivePacket.sys_id + " " + Integer.toUnsignedLong(recivePacket.timestamp));
                        dataString = dataString.trim();
                        
                        if(dataString.equals("YES"))
                        {
                            if((Integer.toUnsignedLong(recivePacket.timestamp) - preTime)> 5)
                            {
                                System.out.println("이상 감지");
                                MainFrame.ap.getJLabel1().setText("패킷 이상 감지");
                                ((DefaultListModel)MainFrame.sp.getList().getModel()).addElement(recivePacket.toString()+"이상 감지");
                                preTime = Integer.toUnsignedLong(recivePacket.timestamp);
                            }
                            
                        }
                        else
                        {
                            System.out.println("패킷 이상 없음");
                            MainFrame.ap.getJLabel1().setText("패킷 이상 없음");
                        }
                        
		}
	}
}
