import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource_CTPPS = cms.ESProducer("XMLIdealCTPPSGeometryESProducer",
                                                rootDDName = cms.string('cms:CMSE'),
                                                label = cms.string(''),
                                                appendToDataLabel = cms.string('XMLIdealGeometryESSource_CTPPS')
                                                )

ctppsGeometryESModule = cms.ESProducer("CTPPSGeometryESModule",
    verbosity = cms.untracked.uint32(1),
    compactViewTag = cms.string('XMLIdealGeometryESSource_CTPPS')
)
