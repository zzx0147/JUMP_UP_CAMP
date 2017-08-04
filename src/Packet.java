
public class Packet {
    int flag;
    int sys_id;
    int sen_id;
    int timestamp;
    byte[] data = {0,0,0,0};//must be casted to char
    
    public Packet(byte[] buffer)
    {
            	
        byte[] tarr=new byte[4];
        System.arraycopy(buffer, 0, tarr, 0, 4);
        //   	byte[] arrtemp={0,0,0,1};
        //   	ByteBuffer wrapped=ByteBuffer.wrap(arrtemp);
        //   	pack.flag=wrapped.getInt();
        this.flag=java.nio.ByteBuffer.wrap(tarr).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();
        //    	System.out.println(pack.flag);
        tarr=new byte[4];
        System.arraycopy(buffer, 4, tarr, 0, 4);
        this.sys_id=java.nio.ByteBuffer.wrap(tarr).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();
        tarr=new byte[4];
        System.arraycopy(buffer, 8, tarr, 0, 4);
        this.sen_id=java.nio.ByteBuffer.wrap(tarr).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();
        tarr=new byte[4];
        System.arraycopy(buffer, 12, tarr, 0, 4);
        this.timestamp=java.nio.ByteBuffer.wrap(tarr).order(java.nio.ByteOrder.LITTLE_ENDIAN).getInt();
        System.arraycopy(buffer, 16, this.data,0,4);
        
        //for(int i=0;i<8;i++){pack.data[i]=java.nio.ByteBuffer.wrap(tarr).order(java.nio.ByteOrder.LITTLE_ENDIAN).getChar(i);}
        //pack.data=java.nio.ByteBuffer.wrap(tarr).order(java.nio.ByteOrder.LITTLE_ENDIAN).getChar();
    } 
    
    
    public Packet(int flag, int sys_id, int sen_id, int timestamp, byte[] data) {
		this.flag = flag;
		this.sys_id = sys_id;
		this.sen_id = sen_id;
		this.timestamp = timestamp;
		for(int i = 0 ; i < 4 ; i++)
                {
                    this.data[i] = data[i];
                }
	}
    
    
    public String toString()
    {
        return "flag: "+ flag + " sys_id: "+sys_id+" sen_id" + sen_id+ " time: "+ Integer.toUnsignedLong(timestamp);
    }
}



