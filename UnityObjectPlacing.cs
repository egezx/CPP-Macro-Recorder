//First Instantiate new temporary object.
placementPosition = player.transform.position + player.transform.forward * 3;
protoModel = GameObject.Instantiate (object2place, placementPosition,player.transform.rotation) as GameObject;


//This function is used to apply green or red material to indicate whether object can be placed to current location.
//Those two materials are standard shaders with transparent redering mode and green/red emission.
private void SetMaterial (GameObject obj,bool allowBuilding)
{
  foreach (Transform child in obj.transform) {
    SetMaterial (child.gameObject, allowBuilding);
  }
  Renderer[] pmRenderers = obj.GetComponents<Renderer> ();
  foreach (var rndr in pmRenderers) {
    if(allowBuilding)
      rndr.material = (Material) Resources.Load ("AlphaGreen") as Material;
    else
      rndr.material = (Material) Resources.Load ("AlphaRed") as Material;
  }
}





//Example from door placing into doorframe:
private void DoorPlacer(string tagName)
{
  RaycastHit rayHit;
  if (canBuild != canBuildOldState) {
    SetMaterial (protoModel, canBuild);
    canBuildOldState = canBuild;
  }
  
  if (Physics.Raycast (player.transform.position + new Vector3 (0, 6, 0), Camera.main.transform.forward, out rayHit)) {
    if (rayHit.collider.tag == "DoorFrame") {
      protoModel.transform.position = rayHit.transform.position + (rayHit.transform.right * 3f) + (rayHit.transform.up * -1.5f);
      protoModel.transform.rotation = rayHit.transform.rotation;
      canBuild = true;
    }
    else {
      canBuild = false;

      protoModel.transform.localEulerAngles = new Vector3(0,player.transform.eulerAngles.y,0);
      protoModel.transform.position = new Vector3 (rayHit.point.x, terrain.SampleHeight (protoModel.transform.position) + 3, rayHit.point.z);
    }
  }
  
  //When player clicks mouse and building is allowed, we just instantiate new object and copy position & location from temporary object.
  if (Input.GetMouseButtonDown (0) && canBuild) {
    GameObject newObject = GameObject.Instantiate (object2place, protoModel.transform.position, protoModel.transform.rotation) as GameObject;
    FormatPlacerObjects ();
  }
}
