import io.netty.bootstrap.Bootstrap;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;

public class ContainerClient {
	String server;
	int port;
	int containerPort;

	public ContainerClient(String server, int port, int containerPort) {
		this.server = server;
		this.port = port;
		this.containerPort = containerPort;
	}

	public void start()
	{
		EventLoopGroup group = new NioEventLoopGroup();
		
		try{
			Bootstrap bootstrap = new Bootstrap().group(group)
					.channel(NioSocketChannel.class)
					.handler(new ClientAdapterInitializer());
			
			UDPThread uThread = new UDPThread();
			uThread.start();
			NioSocketChannel channel = (NioSocketChannel)bootstrap.connect(server,port).sync().channel();
			
			channel.write("client connetion success\n");
			channel.flush();
			while(true)
			{
				channel.write("Hi\n");
				//System.out.println("send : Hi\n");
				channel.flush();
				Thread.sleep(1000);
			}
		}catch(Exception e){
			e.printStackTrace();
		}finally{
			group.shutdownGracefully();
		}
	}
}