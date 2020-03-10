import StardustAPI.Input 1.0

HandBone {
    id:arm
    
    property alias arm: this
    property alias wrist: wrist
    property alias thumb: thumb
    
    HandBone {
        id:wrist
        
        HandBone {
            id:thumb
            
            
        }
        
        HandBone {
            id:index
            
            
        }
        
        HandBone {
            id:middle
            
            
        }
        
        HandBone {
            id:ring
            
            
        }
        
        HandBone {
            id:pinky
            
            
        }
    }
}
