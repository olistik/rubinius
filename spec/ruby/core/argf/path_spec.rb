require File.expand_path('../../../spec_helper', __FILE__)
require File.expand_path('../shared/filename', __FILE__)

describe "ARGF.path" do
  it_behaves_like :argf_filename, :path
end
