/**
 * @author Felix Thielke
 */

#include "../ActivationFunctions.h"
#include "SConv2D.h"

namespace NeuralNetwork
{
  namespace CompiledNNImpl
  {
    void SConv2DLayerCompiler::initialize()
    {
      // Declare constants
      constants.resize(1);
      // Store weights
      NetworkConstants& weights = constants[0];
      weights.data.clear();
      const unsigned int outputBatchSize = 4 * (settings.xmmRegs() - 2);
      for(unsigned int outputOffset = 0; outputOffset < l.outputDimensions[2]; outputOffset += outputBatchSize)
      {
        const unsigned int outputBatchEnd = std::min(outputOffset + outputBatchSize, l.outputDimensions[2]);

        for(unsigned int y = 0; y < l.weights.dims(0); y++)
        {
          for(unsigned int input = 0; input < l.weights.dims(1) ; input += 1)
          {
            for(unsigned int output = outputOffset; output < outputBatchEnd; output += 4)
            {
              const unsigned int remainingOutputs = std::min(4u, outputBatchEnd - output);

              for(unsigned int i = 0; i < remainingOutputs; i++)
              {
                int row = y * l.weights.dims(1) * l.outputDimensions[2];
                int col = input * l.outputDimensions[2];
                weights.data.emplace_back(l.weights[row + col  + output + i]);
              }
              for(unsigned int i = remainingOutputs; i < 4; i++)
                weights.data.emplace_back(0.f);
            }
          }
        }
      }
    }

    size_t SConv2DLayerCompiler::bufferSizeNeeded() const
    {
      return l.padding == PaddingType::same ? (l.outputDimensions[0] * l.strides[0] + l.weights.dims(0)) * (l.outputDimensions[1] * l.strides[1] + l.weights.dims(1)) * l.inputDimensions[2] : 0;
    }

    unsigned int SConv2DLayerCompiler::padImage(X86Assembler& a, const float* const input) const
    {
      enum Side
      {
        TOP,
        LEFT
      };
      const std::array<unsigned int, 2> paddingCount{ {
          ((l.outputDimensions[0] - 1) * l.strides[0] + l.weights.dims(0) - l.inputDimensions[0]) / 2, // Top
          ((l.outputDimensions[1] - 1) * l.strides[1] + l.weights.dims(1) - l.inputDimensions[1]) / 2 // Left
        } };

      if(paddingCount[TOP] == 0 && paddingCount[LEFT] == 0)
        return 0;

      // Pad image
      a.mov(a.zsi(), imm_ptr<const float*>(input + (l.inputDimensions[0] - 1) * l.inputDimensions[1] * l.inputDimensions[2]));
      a.mov(a.zdi(), imm_ptr<const float*>(input + ((l.inputDimensions[0] + paddingCount[TOP] - 1) * (l.inputDimensions[1] + paddingCount[LEFT] * 2) + paddingCount[LEFT]) * l.inputDimensions[2]));
      if(l.inputDimensions[1] * l.inputDimensions[2] % 4 == 0)
      {
        a.mov(a.zax(), imm_u(l.inputDimensions[0]));
        Label copyLoop = a.newLabel();
        a.bind(copyLoop);

        unsigned int stepsRemaining = l.inputDimensions[1] * l.inputDimensions[2] / 4;
        unsigned int pointerOffset = 0;
        for(unsigned int stepSize = settings.xmmRegs(); stepSize; --stepSize)
        {
          if(stepsRemaining < stepSize)
            continue;

          Label copyRowLoop;
          if(stepsRemaining >= 2 * stepSize)
          {
            copyRowLoop = a.newLabel();
            a.mov(a.zcx(), imm_u(stepsRemaining / stepSize));
            a.bind(copyRowLoop);
          }

          for(unsigned int i = 0; i < stepSize; i++)
            a.movaps(x86::xmm(i), a.ptr_zsi(i * 4 * sizeof(float)));
          for(unsigned int i = 0; i < stepSize; i++)
            a.movups(a.ptr_zdi(i * 4 * sizeof(float)), x86::xmm(i));

          if(stepsRemaining == stepSize)
            pointerOffset = stepSize * 4 * sizeof(float);
          else
          {
            a.add(a.zsi(), imm_u(stepSize * 4 * sizeof(float)));
            a.add(a.zdi(), imm_u(stepSize * 4 * sizeof(float)));
          }

          if(stepsRemaining >= 2 * stepSize)
            a.loop(copyRowLoop);

          stepsRemaining %= stepSize;
        }

        a.sub(a.zsi(), imm_u(l.inputDimensions[1] * 2 * l.inputDimensions[2] * sizeof(float) - pointerOffset));
        a.sub(a.zdi(), imm_u((l.inputDimensions[1] + paddingCount[LEFT]) * 2 * l.inputDimensions[2] * sizeof(float) - pointerOffset));

        a.dec(a.zax());
        a.jnz(copyLoop);
      }
      else
      {
        // TODO
        ASSERT(false);
      }

      // Prepare setting borders to zero
      a.mov(a.zsi(), imm_ptr<const float*>(input));
      unsigned int clearRegisters = 0;

      // Clear top border
      unsigned int remainingElements = paddingCount[TOP] * (l.inputDimensions[1] + paddingCount[LEFT] * 2) * l.inputDimensions[2];
      for(unsigned int stepSize = settings.xmmRegs(); stepSize; --stepSize)
      {
        const unsigned int elementsPerStep = stepSize * 4;
        if(remainingElements < elementsPerStep)
          continue;

        if(clearRegisters < stepSize)
        {
          for(unsigned int i = clearRegisters; i < stepSize; i++)
            a.xorps(x86::xmm(i), x86::xmm(i));
          clearRegisters = stepSize;
        }

        Label clearTopLoop;
        if(remainingElements >= elementsPerStep * 2)
        {
          clearTopLoop = a.newLabel();
          a.mov(a.zcx(), imm_u(remainingElements / elementsPerStep));
          a.bind(clearTopLoop);
        }

        for(unsigned int i = 0; i < stepSize; i++)
          a.movaps(a.ptr_zsi(i * 4 * sizeof(float)), x86::xmm(i));

        a.add(a.zsi(), imm_u(stepSize * 4 * sizeof(float)));

        if(remainingElements >= elementsPerStep * 2)
          a.loop(clearTopLoop);

        remainingElements %= elementsPerStep;
      }
      if(remainingElements)
      {
        if(clearRegisters == 0)
        {
          a.xorps(x86::xmm0, x86::xmm0);
          clearRegisters = 1;
        }
        for(unsigned int i = 0; i < remainingElements; i++)
          a.movss(a.ptr_zsi(i * sizeof(float)), x86::xmm(i % clearRegisters));
      }

      // Clear bottom border
      const bool bottomAligned = (l.inputDimensions[0] + paddingCount[TOP]) * (l.inputDimensions[1] + paddingCount[LEFT] * 2) * l.inputDimensions[2] % 4 == 0;
      a.add(a.zsi(), imm_u((l.inputDimensions[0] * (l.inputDimensions[1] + paddingCount[LEFT] * 2) * l.inputDimensions[2] + remainingElements) * sizeof(float)));
      remainingElements = paddingCount[TOP] * (l.inputDimensions[1] + paddingCount[LEFT] * 2) * l.inputDimensions[2];
      for(unsigned int stepSize = settings.xmmRegs(); stepSize; --stepSize)
      {
        const unsigned int elementsPerStep = stepSize * 4;
        if(remainingElements < elementsPerStep)
          continue;

        if(clearRegisters < stepSize)
        {
          for(unsigned int i = clearRegisters; i < stepSize; i++)
            a.xorps(x86::xmm(i), x86::xmm(i));
          clearRegisters = stepSize;
        }

        Label clearBottomLoop;
        if(remainingElements >= elementsPerStep * 2)
        {
          clearBottomLoop = a.newLabel();
          a.mov(a.zcx(), imm_u(remainingElements / elementsPerStep));
          a.bind(clearBottomLoop);
        }

        for(unsigned int i = 0; i < stepSize; i++)
        {
          if(bottomAligned)
            a.movaps(a.ptr_zsi(i * 4 * sizeof(float)), x86::xmm(i));
          else
            a.movups(a.ptr_zsi(i * 4 * sizeof(float)), x86::xmm(i));
        }

        a.add(a.zsi(), imm_u(stepSize * 4 * sizeof(float)));

        if(remainingElements >= elementsPerStep * 2)
          a.loop(clearBottomLoop);

        remainingElements %= elementsPerStep;
      }
      if(remainingElements)
      {
        if(clearRegisters == 0)
        {
          a.xorps(x86::xmm0, x86::xmm0);
          clearRegisters = 1;
        }
        for(unsigned int i = 0; i < remainingElements; i++)
          a.movss(a.ptr_zsi(i * sizeof(float)), x86::xmm(i % clearRegisters));
      }

      // Clear left and right borders to zero
      a.mov(a.zsi(), imm_ptr<const float*>(input + paddingCount[TOP] * (l.inputDimensions[1] + paddingCount[LEFT] * 2) * l.inputDimensions[2]));
      a.mov(a.zcx(), imm_u(l.inputDimensions[0]));
      Label clearLeftAndRightLoop = a.newLabel();
      a.bind(clearLeftAndRightLoop);
      unsigned int paddingRemaining = paddingCount[LEFT] * l.inputDimensions[2];
      unsigned int offset = 0;
      for(; paddingRemaining >= 4; paddingRemaining -= 4)
      {
        a.movups(a.ptr_zsi(offset * sizeof(float)), x86::xmm0);
        a.movups(a.ptr_zsi(((l.inputDimensions[1] + paddingCount[LEFT]) * l.inputDimensions[2] + offset) * sizeof(float)), x86::xmm0);
        offset += 4;
      }
      for(; paddingRemaining; --paddingRemaining)
      {
        a.movss(a.ptr_zsi(offset * sizeof(float)), x86::xmm0);
        a.movss(a.ptr_zsi(((l.inputDimensions[1] + paddingCount[LEFT]) * l.inputDimensions[2] + offset) * sizeof(float)), x86::xmm0);
        offset++;
      }
      a.add(a.zsi(), imm_u((l.inputDimensions[1] + paddingCount[LEFT] * 2) * l.inputDimensions[2] * sizeof(float)));
      a.loop(clearLeftAndRightLoop);

      return paddingCount[LEFT] * 2;
    }

    void SConv2DLayerCompiler::compileFilter(X86Assembler& a, const bool inputAligned, const unsigned int remainingOutputs, const unsigned int remainingInput, const bool lastFilter) const
    {
      const unsigned int stepSize = (remainingOutputs + 3) / 4;

      // Apply filter
      unsigned int filterOffset = 0;
      for(unsigned int step = 0; step < stepSize; step++)
      {
        // Load input values
        if(remainingOutputs == 1)
        {
          a.movss(x86::xmm(settings.xmmRegs() - 1), a.ptr_zdx());
          a.shufps(x86::xmm(settings.xmmRegs() - 1), x86::xmm(settings.xmmRegs() - 1), imm_u(0));
        }
        else
        {
          if(inputAligned)
            a.movaps(x86::xmm(settings.xmmRegs() - 1), a.ptr_zdx());
          else
            a.movups(x86::xmm(settings.xmmRegs() - 1), a.ptr_zdx());

          //if (remainingOutputs < 4)
          //    a.shufps(x86::xmm(settings.xmmRegs() - 1), x86::xmm(settings.xmmRegs() - 1), imm_u(0 | ((1 % remainingOutputs) << 2) | ((2 % remainingOutputs) << 4) | ((3 % remainingOutputs) << 6)));
        }

        if(step == stepSize - 1 && remainingOutputs % 4 == 1)
        {
          a.movss(x86::xmm(settings.xmmRegs() - 2), a.ptr_zbx(filterOffset));
          a.mulss(x86::xmm(settings.xmmRegs() - 2), x86::xmm(settings.xmmRegs() - 1));
          a.addss(x86::xmm(step), x86::xmm(settings.xmmRegs() - 2));
        }
        else
        {
          a.movaps(x86::xmm(settings.xmmRegs() - 2), a.ptr_zbx(filterOffset));
          a.mulps(x86::xmm(settings.xmmRegs() - 2), x86::xmm(settings.xmmRegs() - 1));
          a.addps(x86::xmm(step), x86::xmm(settings.xmmRegs() - 2));
        }
        filterOffset += 4 * sizeof(float);
        a.add(a.zdx(), imm_u(4 * sizeof(float)));
      }

      a.add(a.zbx(), imm_u(filterOffset));
    }

    void SConv2DLayerCompiler::compileOutputBatch(X86Assembler& a, const unsigned int inputWidth, const unsigned int remainingOutputs) const
    {
      const bool outputAligned = l.outputDimensions[2] % 4 == 0;
      const unsigned int stepSize = (remainingOutputs + 3) / 4;

      // Load input base address in zdx
      a.mov(a.zdx(), a.zsi());

      // Initialize filter result
      for(unsigned int step = 0; step < stepSize; step++)
        a.xorps(x86::xmm(step), x86::xmm(step));

      // Begin loop over weight rows
      Label filterRowLoop = a.newLabel();
      a.mov(a.zax(), imm_u(l.weights.dims(0)));
      a.bind(filterRowLoop);

      // Begin loop over weight cols
      Label filterColLoop = a.newLabel();
      a.mov(a.zcx(), imm_u(l.weights.dims(1)));
      a.bind(filterColLoop);
      compileFilter(a, remainingOutputs, remainingOutputs, false);

      // End loop over weight cols
      if(remainingOutputs <= 4)
        a.loop(filterColLoop);
      else
      {
        a.dec(a.zcx());
        a.jnz(filterColLoop);
      }

      // Set input pointer to next row
      a.add(a.zdx(), imm_u(((inputWidth - l.weights.dims(1)) * l.inputDimensions[2] + 0) * sizeof(float)));

      // End loop over weight rows
      a.dec(a.zax());
      a.jnz(filterRowLoop);

      // Store output
      for(unsigned int step = 0; step < stepSize; step++)
      {
        if(step == stepSize - 1 && remainingOutputs % 4 == 1)
          a.movss(a.ptr_zdi(step * 4 * sizeof(float)), x86::xmm(step));
        else if(outputAligned)
          a.movaps(a.ptr_zdi(step * 4 * sizeof(float)), x86::xmm(step));
        else
          a.movups(a.ptr_zdi(step * 4 * sizeof(float)), x86::xmm(step));
      }
      a.add(a.zdi(), imm_u(remainingOutputs * sizeof(float)));
    }

    void SConv2DLayerCompiler::compileSimpleConvolution(X86Assembler& a, const unsigned int inputWidth) const
    {
      const unsigned int inputSize = l.weights.dims(1) * l.inputDimensions[2];

      // Load weights address
      a.lea(a.zbx(), x86::ptr(constants[0].label));

      // Calculate number of needed registers
      unsigned int regsNeeded = 0;
      unsigned int regOffset = 0;
      for(unsigned int remainingRows = l.weights.dims(0); remainingRows;)
      {
        const unsigned int rowsInThisIteration = std::min((settings.xmmRegs() - regOffset) / inputSize, remainingRows);
        regsNeeded = std::max(regsNeeded, rowsInThisIteration * inputSize);
        remainingRows -= rowsInThisIteration;
        if(regOffset == 0)
          regOffset = 1;
      }

      // If some registers are free, load some weights
      const unsigned int weightRegisterOffset = regsNeeded + 1; // TODO check this
      const unsigned int weightRegisterCount = std::min(l.weights.dims(0) * inputSize, static_cast<unsigned int>(std::max(0, static_cast<int>(settings.xmmRegs()) - static_cast<int>(weightRegisterOffset))));
      const unsigned int weightRegisterMemoryOffset = (l.weights.dims(0) * inputSize - weightRegisterCount) * 4 * sizeof(float);
      for(unsigned int i = 0; i < weightRegisterCount; i++)
        a.movaps(x86::xmm(weightRegisterOffset + i), a.ptr_zbx(weightRegisterMemoryOffset + i * 4 * sizeof(float)));

      // Begin loop over rows
      Label rowLoop;
      if(l.outputDimensions[0] > 1)
      {
        a.mov(a.zax(), imm_u(l.outputDimensions[0]));
        rowLoop = a.newLabel();
        a.bind(rowLoop);
      }

      // Begin loop over cols
      Label filterLoop;
      if(l.outputDimensions[1] > 1)
      {
        a.mov(a.zcx(), imm_u(l.outputDimensions[1]));
        filterLoop = a.newLabel();
        a.bind(filterLoop);
      }

      // Apply filter
      unsigned int sourceOffset = 0;
      unsigned int filterOffset = 0;
      regOffset = 0;
      for(unsigned int remainingRows = l.weights.dims(0); remainingRows;)
      {
        const unsigned int rowsInThisIteration = std::min((settings.xmmRegs() - regOffset) / inputSize, remainingRows);

        for(unsigned int filterRow = 0; filterRow < rowsInThisIteration; filterRow++)
        {
          if((l.strides[1] * l.inputDimensions[2]) % 4 == 0)
          {
            if(inputSize < 4)
              a.pshufd(x86::xmm(regOffset + filterRow * inputSize), a.ptr_zsi(sourceOffset), imm_u(0 | ((1 % inputSize) << 2) | ((2 % inputSize) << 4) | ((3 % inputSize) << 6)));
            else
              a.movaps(x86::xmm(regOffset + filterRow * inputSize), a.ptr_zsi(sourceOffset));
          }
          else
          {
            a.movups(x86::xmm(regOffset + filterRow * inputSize), a.ptr_zsi(sourceOffset));
            if(inputSize < 4)
              a.pshufd(x86::xmm(regOffset + filterRow * inputSize), x86::xmm(regOffset + filterRow * inputSize), imm_u(0 | ((1 % inputSize) << 2) | ((2 % inputSize) << 4) | ((3 % inputSize) << 6)));
          }
          sourceOffset += (inputWidth * l.inputDimensions[2]) * sizeof(float);
        }
        for(unsigned int i = 1; i < inputSize; i++)
        {
          for(unsigned int filterRow = 0; filterRow < rowsInThisIteration; filterRow++)
            a.pshufd(x86::xmm(regOffset + filterRow * inputSize + i), x86::xmm(regOffset + filterRow * inputSize + i - 1), imm_u((1 % inputSize) | ((2 % inputSize) << 2) | ((3 % inputSize) << 4) | ((4 % inputSize) << 6)));
        }
        for(unsigned int i = 0; i < rowsInThisIteration * inputSize; i++)
        {
          if(filterOffset >= weightRegisterMemoryOffset)
            a.mulps(x86::xmm(regOffset + i), x86::xmm(weightRegisterOffset + (filterOffset - weightRegisterMemoryOffset) / (4 * sizeof(float))));
          else
            a.mulps(x86::xmm(regOffset + i), a.ptr_zbx(filterOffset));
          filterOffset += 4 * sizeof(float);
        }

        bool odd = (rowsInThisIteration * inputSize) % 2 != 0;
        for(unsigned int stride = (rowsInThisIteration * inputSize) / 2; stride; stride /= 2)
        {
          for(unsigned int i = 0; i < stride; i++)
            a.addps(x86::xmm(i + regOffset), x86::xmm(i + stride + regOffset));
          if(odd)
            a.addps(x86::xmm(regOffset), x86::xmm(stride * 2 + regOffset));
          odd = stride % 2 != 0;
        }
        if(regOffset)
          a.addps(x86::xmm0, x86::xmm1);

        remainingRows -= rowsInThisIteration;
        if(regOffset == 0)
          regOffset = 1;
      }

      // Store result
      if(l.outputDimensions[2] == 1)
        a.movss(a.ptr_zdi(), x86::xmm0);
      else if(l.outputDimensions[2] == 4)
        a.movaps(a.ptr_zdi(), x86::xmm0);
      else
        a.movups(a.ptr_zdi(), x86::xmm0);
      a.add(a.zdi(), imm_u(l.outputDimensions[2] * sizeof(float)));

      // End loop over cols
      if(l.outputDimensions[1] > 1)
      {
        a.add(a.zsi(), imm_u(l.strides[1] * l.inputDimensions[2] * sizeof(float)));
        a.dec(a.zcx());
        a.jne(filterLoop);
      }

      // End loop over rows
      if(l.outputDimensions[0] > 1)
      {
        a.add(a.zsi(), imm_u(((l.strides[0] * inputWidth - l.outputDimensions[1] * l.strides[1]) * l.inputDimensions[2] + (l.outputDimensions[1] <= 1 ? l.strides[1] * l.inputDimensions[2] : 0)) * sizeof(float)));
        a.dec(a.zax());
        a.jne(rowLoop);
      }
    }

    void SConv2DLayerCompiler::compile(X86Assembler& a, ActivationFunctionHandler& afHandler, const float* const input, const float* const output) const
    {
      const NetworkConstants& weights = constants[0];
      unsigned int inputWidth = l.inputDimensions[1];

      // Zero-pad image if 'same' padding is requested
      if(l.padding == PaddingType::same)
        inputWidth += padImage(a, input);

      // Load input/output base addresses
      a.mov(a.zsi(), imm_ptr<const float*>(input));
      if(isInplace())
        a.mov(a.zdi(), a.zsi());
      else
        a.mov(a.zdi(), imm_ptr<const float*>(output));

      if(l.outputDimensions[2] <= 4 && l.weights.dims(1) * l.inputDimensions[2] <= 4)
        compileSimpleConvolution(a, inputWidth);
      else
      {
        // Begin loop over output image rows
        a.mov(a.ptr_zbp(-4, 4), imm_u(l.outputDimensions[0]));
        Label inputRowLoop = a.newLabel();
        a.bind(inputRowLoop);

        // Begin loop over output image cols
        if(l.outputDimensions[2] / (4 * (settings.xmmRegs() - 2)) < 2 && l.weights.dims(1) * l.inputDimensions[2] <= 4)
          a.mov(a.zax(), imm_u(l.outputDimensions[1]));
        else
          a.mov(a.ptr_zbp(-8, 4), imm_u(l.outputDimensions[1]));
        Label inputColLoop = a.newLabel();
        a.bind(inputColLoop);

        // Load filter base address
        a.lea(a.zbx(), x86::ptr(weights.label));

        biasOffset = 0;

        if(l.outputDimensions[2] > 4 * (settings.xmmRegs() - 2))
        {
          // Begin loop over output batches (only construct loop if it has more than one iteration)
          Label outputBatchLoop;
          if(l.outputDimensions[2] / (4 * (settings.xmmRegs() - 2)) >= 2)
          {
            outputBatchLoop = a.newLabel();
            if(l.weights.dims(1) * l.inputDimensions[2] > 4)
              a.mov(a.ptr_zbp(-12, 4), imm_u(l.outputDimensions[2] / (4 * (settings.xmmRegs() - 2))));
            else
              a.mov(a.zax(), imm_u(l.outputDimensions[2] / (4 * (settings.xmmRegs() - 2))));
            a.bind(outputBatchLoop);
          }

          compileOutputBatch(a, inputWidth, 4 * (settings.xmmRegs() - 2));

          // End loop over output batches
          if(l.outputDimensions[2] / (4 * (settings.xmmRegs() - 2)) >= 2)
          {
            if(l.weights.dims(1) * l.inputDimensions[2] > 4)
              a.dec(a.ptr_zbp(-12, 4));
            else
              a.dec(a.zax());
            a.jnz(outputBatchLoop);
          }
        }

        const unsigned int remainingOutputs = l.outputDimensions[2] == (4 * (settings.xmmRegs() - 2)) ? (4 * (settings.xmmRegs() - 2)) : l.outputDimensions[2] % (4 * (settings.xmmRegs() - 2));
        if(remainingOutputs)
          compileOutputBatch(a, inputWidth, remainingOutputs);

        // Set input offset to next column, respecting the stride
        a.add(a.zsi(), imm_u(l.strides[1] * l.inputDimensions[2] * sizeof(float)));

        // End loop over output image cols
        if(l.outputDimensions[2] / (4 * (settings.xmmRegs() - 2)) < 2 && l.weights.dims(1) * l.inputDimensions[2] < 4)
          a.dec(a.zax());
        else
          a.dec(a.ptr_zbp(-8, 4));
        a.jnz(inputColLoop);

        // Set input offset to next row, respecting the stride
        a.add(a.zsi(), imm_u((l.strides[0] * inputWidth - l.outputDimensions[1] * l.strides[1]) * l.inputDimensions[2] * sizeof(float)));

        // End loop over output image rows
        a.dec(a.ptr_zbp(-4, 4));
        a.jnz(inputRowLoop);
      }
    }
  }
}
