module ESP32
  module STANDARD
    include Constants
    puts "TEST........GPIO 08 GIT"
    puts "TEST........PWM  02 GIT"
    
    class << self
     alias :digital_write :digitalWrite   
      alias :digital_read  :digitalRead
      alias :analog_write  :analogWrite   
      alias :analog_read   :analogRead    
      alias :pin_mode      :pinMode 
    end  
    
    class GPIO
      puts "Test PIN"
      PIN_MODE = {
        pullup:   ESP32::STANDARD::INPUT_PULLUP,
        pulldown: ESP32::STANDARD::INPUT_PULLDOWN,
        input:    ESP32::STANDARD::INPUT,
        output:   ESP32::STANDARD::OUTPUT,
        inout:    ESP32::STANDARD::INPUT_OUTPUT
      }
      #-----------------------class method-----------------------------#
      def self.setmode pin,mode = :input
        puts "setmode"
        #puts pin
        mode = PIN_MODE[mode] unless mode.is_a?(Integer)
        @pin = pin
        @mode = mode
        #self.mode= mode
      end

      def self.read_at pin
        #STANDARD.analog_read pin
        STANDARD.digital_read pin
      end

      def self.high_at? pin
        level = STANDARD.digital_read pin
        if level == 1
          val_read = true
        else 
          val_read = false
        end
        #puts val_read
        return val_read
      end

      def self.low_at? pin
        level = STANDARD.digital_read pin
        if level == 0
          val_read = true
        else 
          val_read = false
        end
        #puts val_read
        return val_read
      end

      def self.write_at pin,val
        puts "TEST digital_write"
        if val == 1
          val_s = HIGH
        elsif val == 0
          val_s = LOW
        end
        STANDARD.pin_mode pin, @mode
        STANDARD.digital_write pin, val_s
        return val
      end 

     
      #-----------------------instance method-----------------------------#
      attr_reader :pin

      def initialize pin, mode = :input
      puts "initialize"
      #puts "setmode"
      #puts pin
      mode = PIN_MODE[mode] unless mode.is_a?(Integer)
      @pin = pin
      @mode = mode
      #self.mode= mode
      end

      def setmode mode = :input # setmode( param ) -> Nil
        puts "setmode"
        #puts pin
        mode = PIN_MODE[mode] unless mode.is_a?(Integer)
        #@pin = pin
        @mode = mode
        #self.mode= mode
      end
    
      def read #Returns the read value as 0 or 1.
        #puts pin
        #STANDARD.analog_read pin
        STANDARD.digital_read pin
      end
      
      def high? # Returns true if the read value is high level (==1).
        level = STANDARD.digital_read pin
        if level == 1
          val_read = true
        else 
          val_read = false
        end
        #puts val_read
        return val_read
      end

      def low? # Returns true if the read value is low level (==0).
        level = STANDARD.digital_read pin
        if level == 0
          val_read = true
        else 
          val_read = false
        end
        #puts val_read
        return val_read
      end

      def write val # write( integer_data )
        puts "TEST write"
        if val == 1
          val_s = HIGH
        elsif val == 0
          val_s = LOW
        end

        self.mode= @mode
        STANDARD.digital_write pin, val_s
        return val
      end 

      ################################################################################
      def analog_write val
        STANDARD.analog_write pin, val
      end
    
      def write val
        puts "TEST digital_write"
        self.mode= @mode
        STANDARD.digital_write pin, val
        val
      end 

      def high!
        puts "HIGH"
        self.mode= @mode
        write HIGH
      end
      
      def low!
        puts "LOW"
        self.mode= @mode
        write LOW 
      end
      
      def off
        low!
      end
      
      def on
        high!
      end
    
      def mode= mode
        puts "TEST PIN MODE"
        STANDARD.pin_mode pin, mode
      end
    
      alias :digital_write :write   
      alias :digital_read  :read
 

      def run
        
        loop {
          write HIGH
          puts "High"
          sleep 1
          write LOW
          puts "Low"
          sleep 1
        }
      end
    ################################################################################

      # the following only work if GPIO_MODE_INPUT_OUTPUT ie, Pin.new(io_num, :inout)
      def toggle
        write((read==HIGH) ? LOW : HIGH)
        puts "toggle"
      end
    end
    
    ################################################################################
    
    class ADC
      #-----------------------class method-----------------------------# 
      def self.read_at pin
        #puts "class method"
        @pin = pin
        #return V
        #STANDARD.analog_read pin
        w = STANDARD.analog_read @pin
        V = w * (3.3 / 4095) # Bits of the ADC is 12
        return V
      end
      
      #-----------------------instance method-----------------------------#
      def initialize pin, mode = :input
        puts "initialize ADC"
        #puts "setmode"
        #puts pin
        mode = PIN_MODE[mode] unless mode.is_a?(Integer)
        @pin = pin
        @mode = mode
        #self.mode= mode
      end
      def test pin, mode
        puts "TEST"
        puts pin
        puts mode
      end

      def read
        #return V
        #STANDARD.analog_read @pin
        w = STANDARD.analog_read @pin
        V = w * (3.3 / 4095) # Bits of the ADC is 12
        return V
      end

      def read_temp
        for i in 0..20
          w = STANDARD.analog_read @pin
          V = w * (3.3 / 4095) # Bits of the ADC is 12
          #puts "Volt : #{V}"
          T = (V - 0.5) /0.01295 # Temperature Coefficient mV/°C
          T = T.round(2)
          puts "Temp : #{T}"
          sleep 1
        end 
      end
      ################################################################## test2
      #def read_at pin 
        #STANDARD.analog_read pin
        #STANDARD.digital_read pin
      #end
    end

    class PWM
      def initialize chan,pin
        puts "initialize PWM"
        #puts "setmode"
        duty = 0
        duty= ((2**13)-1)*duty/100.to_f
        duty = duty.round

        @chan = chan
        @pin = pin
        @freq = 50 # Frequency in Hertz. Set frequency at 50 Hz
        @duty = duty #Set duty cycle at 0%
        STANDARD.pwm @chan,@pin,@freq,@duty
        #self.mode= mode
      end
      #def frequency freq
       #puts "TEST Freq"
      #end

      def frequency freq_n
        #puts @chan
        #puts @pin
        #puts freq_n
        #puts @duty
        @freq = freq_n
        STANDARD.pwm @chan,@pin,@freq,@duty
      end

      def period_us mc
        #out = (1/mc)*(10**6)
        #puts out
        i = (1/mc.to_f)*(10**6.to_f)
        i = i.round
        #puts i
        @freq = i
        STANDARD.pwm @chan,@pin,@freq,@duty
      end

      def duty duty_n
        duty_n = ((2**13)-1)*duty_n/100.to_f
        @duty = duty_n.round
        
        STANDARD.pwm @chan,@pin,@freq,@duty
      end
    end
  end
end
