if ENV['BUILD_PLATFORM'] == 'XCode'
  # XCode requires 'note: ' for this to be output to the console
  def puts(s)
    super("note: " + s.to_s)
  end

  # XCode doesn't support colors
  class String
    def blue
      return self
    end
    def red
      return self
    end
    def grey
      return self
    end
  end
else
  require 'colorize'
end
