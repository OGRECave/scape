#include "mainwindow.h"
#include "EngineCore/EngineInterface.h"
#include "aboutdialog.h"

void MainWindow::aboutApp()
{
    AboutDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        return;
}

void MainWindow::importImage()
{
    openImportImageDialog();
}

void MainWindow::exportImage()
{
    openExportImageDialog();
}

void MainWindow::pencilEraserGPU()
{
    selectTool("HeightfieldOperationGPUPencil", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::directionalNoiseGPU()
{
    selectTool("HeightfieldOperationGPUNoiseDrag", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::noiseGPU()
{
    selectTool("HeightfieldOperationGPUNoise", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::softenGPU()
{
    selectTool("HeightfieldOperationGPUBrush", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::sharpenSoften()
{
    selectTool("HeightfieldOperationCPUSmooth", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::noiseCPU()
{
    selectTool("HeightfieldOperationCPUNoise", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::levelUnlevel()
{
    selectTool("HeightfieldOperationCPULevel", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::waterErosion()
{
    selectTool("HeightfieldOperationCPUErosion", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::pencilEraser()
{
    selectTool("HeightfieldOperationCPUBrush", ScapeEngine::SCAPEUIELEMENTGROUPID_OPERATION);
}

void MainWindow::materialProps()
{
    selectTool("MaterialSettings", ScapeEngine::SCAPEUIELEMENTGROUPID_MATERIAL);
}

void MainWindow::skyProps()
{
    selectTool("SkySettings", ScapeEngine::SCAPEUIELEMENTGROUPID_SKY);
}

void MainWindow::renderwindowProps()
{
    selectTool("RenderWindowSettings", ScapeEngine::SCAPEUIELEMENTGROUPID_RENDERWINDOW);
}

void MainWindow::createPreset()
{
    //
}

void MainWindow::savePreset()
{
    //
}

void MainWindow::deletePreset()
{
    //
}

void MainWindow::exportPreset()
{
    //
}

void MainWindow::importPreset()
{
    //
}

void MainWindow::exitApp() { close(); }
