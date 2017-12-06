require_relative "../test_helper"

module Numo::CUDA
  class NVRTCTest < Test::Unit::TestCase
    def test_nvrtcVersion
      major, minor = NVRTC.nvrtcVersion
      assert { major.is_a?(Integer) }
      assert { minor.is_a?(Integer) }
    end

    def test_nvrtcCreateProgram
      src = "__global__ void k() {}\n"
      name = "simple.cu"
      headers = []
      include_names = []
      assert_nothing_raised do
        prog = NVRTC.nvrtcCreateProgram(src, name, headers, include_names)
      end
    end

    def test_nvrtcDestroyProgram
      prog = test_nvrtcCreateProgram
      assert_nothing_raised do
        NVRTC.nvrtcDestroyProgram(prog)
      end
    end

    def test_nvrtcCompileProgram
      prog = test_nvrtcCreateProgram
      options = []
      assert_nothing_raised do
        NVRTC.nvrtcCompileProgram(prog, options)
      end
      prog
    end

    def test_nvrtcGetPTX
      prog = test_nvrtcCompileProgram
      ptx = NVRTC.nvrtcGetPTX(prog)
      assert { ptx =~ /Generated by NVIDIA NVVM Compiler/ }
    end

    def test_nvrtcGetProgramLog
      prog = test_nvrtcCompileProgram
      log = NVRTC.nvrtcGetProgramLog(prog)
      assert { log.is_a?(String) }
    end
  end
end