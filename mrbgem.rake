MRuby::Gem::Specification.new('mruby-standard-gpio') do |spec|
  spec.license = 'MIT'
  spec.authors = 'SHattapat'

  spec.cc.include_paths << "#{build.root}/src"
end
