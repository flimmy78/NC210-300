/*******************************************************************************
* Description  : ���㲨�β���
* Author       : 2018/4/12 ������, by redmorningcn
*******************************************************************************/


/*******************************************************************************
* INCLUDES
*/
#include <app_wave_task.h>


/*******************************************************************************
* Description  : ͨ��ʱ���������
                 ����ͨ��buf�Ķ�дָ�룬ȷ���Ƿ���Ҫ������
* Author       : 2018/3/13 ���ڶ�, by redmorningcn
*******************************************************************************/
void    app_calc_ch_timepara(void)
{
    uint8   p_write;
    uint8   p_read;
    uint64  starttime;
    uint64  endtime;
    
    uint64  periodtime;
    uint64  ratiotime;
    
    uint8   i;
        
    for(i = 0;i< 2;i++)
    {
        p_write = ch.test[i].p_write;
        p_read  = ch.test[i].p_read;
        
        /*******************************************************************************
        * Description  : �ٶ�ͨ��ʱ���������
        * Author       : 2018/3/13 ���ڶ�, by redmorningcn
        *******************************************************************************/
        if(     ( p_write > p_read) &&  (p_write > p_read+10)           
           ||   ( p_write < p_read) &&  (p_write + CH_TIMEPARA_BUF_SIZE > p_read+10)           
               )  
        {

            /*******************************************************************************
            * Description  : ��������(0.01us) �����ź�����һ���ٴγ��֣�ȡlow_up�ж�Ϊ��׼
            * Author       : 2018/3/13 ���ڶ�, by redmorningcn
            *******************************************************************************/
            p_read      =   ch.test[i].p_read;
            starttime   =   ch.test[i].time[p_read].low_up_time  * 65536 
                        +   ch.test[i].time[p_read].low_up_cnt  ;
            
            p_read      =   (ch.test[i].p_read + 1) % CH_TIMEPARA_BUF_SIZE;       //��ֹԽ��
            endtime     =   ch.test[i].time[p_read].low_up_time  * 65536 
                        +   ch.test[i].time[p_read].low_up_cnt  ;
            
            if(starttime > endtime)             //��ֹ��ת
            {
                endtime += 65536;
            }
            periodtime = endtime - starttime;
            
            ch.para[i].period = (periodtime * 1000*1000*100 )/ sys.cpu_freq;
            
            if(periodtime){
                ch.para[i].freq = sys.cpu_freq  / periodtime;   //����Ƶ��
                
                if(((sys.cpu_freq *10) % periodtime)> 4 )       //��������
                    ch.para[i].freq += 1;
            }
            
            /*******************************************************************************
            * Description  : ����ռ�ձ�(xx.xx%)��( hig_down -  low_up ) / period
            * Author       : 2018/3/13 ���ڶ�, by redmorningcn
            *******************************************************************************/
            p_read      =   ch.test[i].p_read;
            starttime   =   ch.test[i].time[p_read].low_up_time  * 65536 
                        +   ch.test[i].time[p_read].low_up_cnt  ;
            endtime     =   ch.test[i].time[p_read].low_down_time  * 65536 
                        +   ch.test[i].time[p_read].low_down_cnt  ;            
            if(starttime > endtime)             //��ֹ��ת
            {
                endtime += 65536;
            }
            ratiotime = endtime - starttime;
            
            if( periodtime )
                ch.para[i].ratio = ( ratiotime * 100* 100 ) / periodtime; 
            
            /*******************************************************************************
            * Description  : ���������أ�0.01us��
            * Author       : 2018/3/13 ���ڶ�, by redmorningcn
            *******************************************************************************/
            p_read      =   ch.test[i].p_read;
            starttime   =   ch.test[i].time[p_read].low_up_time  * 65536 
                        +   ch.test[i].time[p_read].low_up_cnt  ;
            endtime     =   ch.test[i].time[p_read].hig_up_time  * 65536 
                        +   ch.test[i].time[p_read].hig_up_cnt  ;            
            if(starttime > endtime)             //��ֹ��ת
            {
                endtime += 65536;
            }

            ch.para[i].raise = ( endtime - starttime)/sys.cpu_freq;
            
            /*******************************************************************************
            * Description  : �����½���(0.01us)
            * Author       : 2018/3/13 ���ڶ�, by redmorningcn
            *******************************************************************************/
            p_read      =   ch.test[i].p_read;
            starttime   =   ch.test[i].time[p_read].hig_down_time  * 65536 
                        +   ch.test[i].time[p_read].hig_down_cnt  ;
            endtime     =   ch.test[i].time[p_read].low_down_time  * 65536 
                        +   ch.test[i].time[p_read].low_down_cnt  ;            
            if(starttime > endtime)             //��ֹ��ת
            {
                endtime += 65536;
            }
            
            ch.para[i].fail = ( endtime - starttime)/sys.cpu_freq;
            

            /*******************************************************************************
            * Description  : ������λ��(xx.xx��)
            * Author       : 2018/3/13 ���ڶ�, by redmorningcn
            *******************************************************************************/
            if(i == 1)      
            {
                p_read      =   ch.test[i].p_read;
                starttime   =   ch.test[0].time[p_read].low_up_time  * 65536 
                            +   ch.test[0].time[p_read].low_up_cnt  ;
                endtime     =   ch.test[1].time[p_read].low_up_time  * 65536 
                            +   ch.test[1].time[p_read].low_up_cnt  ;            
                if(starttime > endtime)             //��ֹ��ת
                {
                    endtime += periodtime;          //��һ����ʱ��
                }
                
                ch.ch1_2phase = (endtime - starttime) / periodtime; 
            }

            //��ָ��++
            ch.test[i].p_read++ ;
            ch.test[i].p_read %= CH_TIMEPARA_BUF_SIZE; 
            
            //ȡ�źŵĸߵ͵�ƽ
        }
    }
}


/*******************************************************************************
* Description  : ȡ�źŵ�ƽ���ֵ
* Author       : 2018/3/29 ������, by redmorningcn
*******************************************************************************/
void    app_calc_ch_voltagepara(void)
{
    uint8   i;
    static  uint8   lockflg[2]={0,0};   
    uint16  p_wr;
    uint16  p_rd;
    
    for(i = 0;i< 2;i++)
    {
        /*******************************************************************************
        * Description  : ������������λ�ã��ɼ��ߵ͵�ƽ
                        �ߵ�ƽ�������أ�90%��ɼ���
                        �͵�ƽ���½��أ�10%��ɼ���
                        �ڸߡ��͵�ƽ���ɼ�һ�κ�lockflgʵ�֣����Ž����������ݲɼ���
        * Author       : 2018/3/29 ������, by redmorningcn
        *******************************************************************************/
        p_wr = ch.test[i].p_wr_vol;

        
        if(ch.test[i].station.fall_10 && lockflg[i] == 0 )    
        {
            lockflg[i] = 1;                                     //�����ź�
           
            ch.test[i].voltage[p_wr].ch_low_voltage =   Get_ADC(ADC_Channel_10+i);
        }
        
        //if(ch.test[i].station.raise_90 && lockflg[i]  )
        if(ch.test[i].station.raise_10 && lockflg[i]  )
        {
            lockflg[i] = 0;                                    //�����ź�,����ȡ��͵�ƽ����ִ��
            
            ch.test[i].voltage[p_wr].ch_hig_voltage = Get_ADC(ADC_Channel_10+i);
            
            /*******************************************************************************
            * Description  : ȡ�����ƽ��дָ��++
            * Author       : 2018/3/29 ������, by redmorningcn
            *******************************************************************************/
            if(i == 1)  //ȡ�����ƽ
                ch.test[i].voltage[p_wr].vcc_hig_voltage = Get_ADC(ADC_Channel_10+2);
            
            p_wr++;
            
            ch.test[i].p_wr_vol = p_wr % CH_VOLTAGE_BUF_SIZE;
        }
        
        /*******************************************************************************
        * Description  : ����ߵ͵�ƽ
        * Author       : 2018/3/29 ������, by redmorningcn
        *******************************************************************************/
        p_wr = ch.test[i].p_wr_vol;
        p_rd = ch.test[i].p_rd_vol;

        if(     ( p_wr > p_rd) &&  (p_wr > p_rd+10)           
           ||   ( p_wr < p_rd) &&  (p_wr + CH_VOLTAGE_BUF_SIZE > p_rd+10)           
            )  
        {
            uint32  sum;
            uint16  max,min;
            uint8   tmp8;
            uint16  tmp16;
            
            /*******************************************************************************
            * Description  : ��10�����У���ȥ���ֵ����Сֵ����ȡƽ��
            * Author       : 2018/3/29 ������, by redmorningcn
            *******************************************************************************/
            //����͵�ƽ
            tmp8 = 0;
            sum  = 0;
            max  = ch.test[i].voltage[p_rd].ch_low_voltage;
            min  = max;
            for(tmp8 = 0;tmp8< 10;tmp8++)
            {
                tmp16 = ch.test[i].voltage[(p_rd + tmp8)%CH_VOLTAGE_BUF_SIZE].ch_low_voltage;
                if(tmp16 > max)
                    max = tmp16;
                
                if(tmp16 < min)
                    min = tmp16;
                
                sum += tmp16;
            }
            ch.para[i].Vol = (sum - max - min)/8;
                
            //����ߵ�ƽ
            tmp8 = 0;
            sum  = 0;
            max  = ch.test[i].voltage[p_rd].ch_hig_voltage;
            min  = max;
            for(tmp8 = 0;tmp8< 10;tmp8++)
            {
                tmp16 = ch.test[i].voltage[(p_rd + tmp8)%CH_VOLTAGE_BUF_SIZE].ch_hig_voltage;
                if(tmp16 > max)
                    max = tmp16;
                
                if(tmp16 < min)
                    min = tmp16;
                
                sum += tmp16;
            }
            ch.para[i].Voh = (sum - max - min)/8;
            
            
            //���㹩���Դ
            if(i == 1)
            {
                tmp8 = 0;
                sum  = 0;
                max  = ch.test[i].voltage[p_rd].vcc_hig_voltage;
                min  = max;
                for(tmp8 = 0;tmp8< 10;tmp8++)
                {
                    tmp16 = ch.test[i].voltage[(p_rd + tmp8)%CH_VOLTAGE_BUF_SIZE].vcc_hig_voltage;
                    if(tmp16 > max)
                        max = tmp16;
                    
                    if(tmp16 < min)
                        min = tmp16;
                    
                    sum += tmp16;
                }
                
                ch.vcc_vol = (sum - max - min)/8;
            }
            
            /*******************************************************************************
            * Description  : ������ָ��
            * Author       : 2018/3/29 ������, by redmorningcn
            *******************************************************************************/
            ch.test[i].p_rd_vol = (p_rd + tmp8) % CH_VOLTAGE_BUF_SIZE;
        }
    }
}


/*******************************************************************************
* Description  : �ٶ��źŵ��쳣�ж�
* Author       : 2018/4/13 ������, by redmorningcn
*******************************************************************************/
void    app_ch_judge(void)
{
    static  uint32  tick;
    static  uint32  plusecnt[2];
    static  uint32  errcnt[2];
    uint8   i;
    
    if(sys.time > tick + 300 || sys.time < tick)       //300ms�ж�һ��
    {
        tick = sys.time;
            
        for(i = 0; i < 2;i++)
        {
            if(ch.test[i].pulse_cnt > plusecnt[i])
            {
                errcnt[i] = 0;
                ch.para[i].status       &= ~(0x01);     //�����ź�����  
                
            }
            else
            {
                errcnt[i]++;
                if(errcnt[i] > 2)                       //�쳣������(�ñ�ʶλ���������㣬�������ź�)
                {
                    ch.para[i].fail     = 0;
                    ch.para[i].freq     = 0;
                    ch.para[i].period   = 0;
                    ch.para[i].raise    = 0;
                    ch.para[i].ratio    = 0;
                    ch.para[i].status  |= 0x01;         //�������ź�
                    ch.para[i].Voh      = 0;
                    ch.para[i].Vol      = 0;
                    
                    if(i == 1){
                        ch.ch1_2phase   = 0;
                        ch.vcc_vol      = 0;
                    }
                    
                }
            }
            plusecnt[i] = ch.test[i].pulse_cnt;
        }
    }
    
}


/*******************************************************************************
* 				end of file
*******************************************************************************/
