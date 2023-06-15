module ESP32
  module STANDARD
    include Constants
    puts "TEST........GPIO 06 GIT"
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
      

      attr_reader :pin
      def initialize pin, mode = :input
      puts "initialize"
      puts "setmode"
      puts pin
      mode = PIN_MODE[mode] unless mode.is_a?(Integer)
      @pin = pin
      @mode = mode
      #self.mode= mode
    end

      def setmode pin, mode = :input
        puts "setmode"
        puts pin
        mode = PIN_MODE[mode] unless mode.is_a?(Integer)
        @pin = pin
        @mode = mode
        #self.mode= mode
      end

      def analog_read
        STANDARD.analog_read pin
      end

      def read_at pin
        STANDARD.analog_read pin
      end
    
      def read
        STANDARD.digital_read pin
      end 
     
      def analog_write val
        STANDARD.analog_write pin, val
      end
    
      def write val
        puts "TEST digital_write"
        self.mode= @mode
        STANDARD.digital_write pin, val
        val
      end 

      def digita val
        puts "TEST digital_write"
        self.mode= @mode
        STANDARD.digital_write pin, val
        val
      end 

      def high_at? 
        puts "test05"
      end

      def low_at? 
        puts "test06"
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
        #puts "TEST PIN MODE"
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
    
      # the following only work if GPIO_MODE_INPUT_OUTPUT ie, Pin.new(io_num, :inout)
      def toggle
        write((read==HIGH) ? LOW : HIGH)
        puts "toggle"
      end
    end
  end
end
