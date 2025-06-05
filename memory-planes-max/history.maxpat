{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 9,
			"minor" : 0,
			"revision" : 7,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 142.0, 150.0, 1000.0, 755.0 ],
		"gridsize" : [ 15.0, 15.0 ],
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-29",
					"lastchannelcount" : 21,
					"maxclass" : "mc.live.gain~",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "multichannelsignal", "", "float", "list" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 371.0, 243.0, 146.0, 50.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_longname" : "mc.live.gain~[32]",
							"parameter_mmax" : 6.0,
							"parameter_mmin" : -70.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "cicades",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"varname" : "mc.live.gain~[5]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "multichannelsignal" ],
					"patching_rect" : [ 371.0, 211.0, 153.0, 22.0 ],
					"text" : "mem.memory 3 cicadas 43"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-24",
					"lastchannelcount" : 21,
					"maxclass" : "mc.live.gain~",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "multichannelsignal", "", "float", "list" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 199.0, 243.0, 146.0, 50.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_longname" : "mc.live.gain~[23]",
							"parameter_mmax" : 6.0,
							"parameter_mmin" : -70.0,
							"parameter_modmode" : 3,
							"parameter_shortname" : "insects",
							"parameter_type" : 0,
							"parameter_unitstyle" : 4
						}

					}
,
					"varname" : "mc.live.gain~[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "multichannelsignal" ],
					"patching_rect" : [ 199.0, 211.0, 149.0, 22.0 ],
					"text" : "mem.memory 2 insects 22"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-29", 0 ],
					"source" : [ "obj-30", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-24" : [ "mc.live.gain~[23]", "insects", 0 ],
			"obj-25::obj-112" : [ "mc.live.gain~[35]", "reverb", 0 ],
			"obj-25::obj-13" : [ "mc.live.gain~[6]", "reverb", 0 ],
			"obj-25::obj-39" : [ "mc.live.gain~[30]", "memory echo", 0 ],
			"obj-25::obj-92" : [ "mc.live.gain~[34]", "memory echo", 0 ],
			"obj-29" : [ "mc.live.gain~[32]", "cicades", 0 ],
			"obj-30::obj-112" : [ "mc.live.gain~[1]", "reverb", 0 ],
			"obj-30::obj-13" : [ "mc.live.gain~[7]", "reverb", 0 ],
			"obj-30::obj-39" : [ "mc.live.gain~[2]", "memory echo", 0 ],
			"obj-30::obj-92" : [ "mc.live.gain~[3]", "memory echo", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"parameter_overrides" : 			{
				"obj-30::obj-112" : 				{
					"parameter_longname" : "mc.live.gain~[1]"
				}
,
				"obj-30::obj-13" : 				{
					"parameter_longname" : "mc.live.gain~[7]"
				}
,
				"obj-30::obj-39" : 				{
					"parameter_longname" : "mc.live.gain~[2]"
				}
,
				"obj-30::obj-92" : 				{
					"parameter_longname" : "mc.live.gain~[3]"
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "mem.forLoop.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.inchesToMeters.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.memory.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.polyMemory.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.radialPan.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.spatConfig.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.spatReverb.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.stutter.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "mem.wfsReverbPositions.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "relativetoabsolute.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "spat5.osc.prepend.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "spat5.pan~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "thru.maxpat",
				"bootpath" : "C74:/patchers/m4l/Pluggo for Live resources/patches",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "wfs.formatSpat.maxpat",
				"bootpath" : "~/Documents/of_v0.12.0_osx_release/apps/myApps/memory-planes/memory-planes-max",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
