<?php 

// affili.net API Calls

// Username and passwords
define ("USERNAME", "736510"); // the publisher ID
define ("PASSWORD", "04H4XejELLoxI69SUFIl"); // the publisher web services password
define ("PRODPASSWORD", "n7u8ueQIATdC1RXcyCbn"); // the product web services password

// API
define ("WSDL_LOGON", "https://api.affili.net/V2.0/Logon.svc?wsdl");
define ("WSDL_SERVICE",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_STATS",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_PROD",  "https://api.affili.net/V2.0/ProductServices.svc?wsdl");
define ("WSDL_CREAT",  "https://api.affili.net/V2.0/PublisherCreative.svc?wsdl");
define ("WSDL_WS",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_REQ",  "https://api.affili.net/V2.0/PublisherProgram.svc?wsdl");
define ("WSDL_DATA",  "http://product-api.affili.net/V3/WSDLFactory/Product_ProductData.wsdl");
define ("WSDL_DATA_LOGON",  "http://product-api.affili.net/Authentication/Logon.svc?wsdl");

function _affilinet_GetMyPrograms( $query) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$params = array('Query' => $query);

	$SOAP_REQUEST = new SoapClient(WSDL_STATS);
	$req = $SOAP_REQUEST->GetMyPrograms(array(
				'CredentialToken' => $Token,
				'GetProgramsRequestMessage' => $params
				));

	return $req;
}


function _affilinet_GetProgramRates($ProgramId) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$SOAP_REQUEST = new SoapClient(WSDL_SERVICE); $req = $SOAP_REQUEST->GetProgramRates(array(
				'CredentialToken' => $Token,
				'PublisherId' => USERNAME,
				'ProgramId' => $ProgramId
			));
	 
	return $req;
}

function _affilinet_GetShopList() {
	$SOAP_PROD_LOGON = new SoapClient(WSDL_LOGON);
	$ProdToken      = $SOAP_PROD_LOGON->Logon(array(
				  'Username'  => USERNAME,
				  'Password'  => PRODPASSWORD,
				  'WebServiceType' => 'Product'
				  ));
	 
	$SOAP_PROD_REQUEST = new SoapClient(WSDL_PROD);
	$req = $SOAP_PROD_REQUEST->GetShopList(array(
				 'CredentialToken' => $ProdToken
				 ));
	 
		foreach ($req->Shops->Shop as $row) {
			print "<b>" . $row->Title . "</b><br>";
			print "<p><img src='" . $row->LogoUrl . "'></p>";
			print "<p>Produkte: " . $row->Products . "</p>";
			print "<hr>";
		}
		
		return $req;
}

function _affilinet_SearchCreatives($ProgramIds) { //muss ein array sein
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$DisplaySettings = array (
				'PageSize' => 10,
				'CurrentPage' => 1
				);
	$SearchCreativesQuery = array(
				'ProgramIds' => $ProgramIds
				);
				
	$SOAP_REQUEST = new SoapClient(WSDL_CREAT);
	$req = $SOAP_REQUEST->SearchCreatives(array(
				'CredentialToken' => $Token,
				'DisplaySettings' => $DisplaySettings,
				'SearchCreativesQuery' => $SearchCreativesQuery
				));
	return $req;
}

function _affilinet_GetProgramCategories() {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));

	$SOAP_REQUEST = new SoapClient(WSDL_WS);
	$req = $SOAP_REQUEST->GetProgramCategories($Token);

	return $req;
}

function _affilinet_GetProgramListByCategory($CategoryId) {
	$SOAP_LOGON = new SoapClient(WSDL_LOGON);
	$Token      = $SOAP_LOGON->Logon(array(
				 'Username'  => USERNAME,
				 'Password'  => PASSWORD,
				 'WebServiceType' => 'Publisher'
				 ));
	$params = array(
			  'CategoryId' => 9
			  );

	$SOAP_REQUEST = new SoapClient(WSDL_REQ);
	$req = $SOAP_REQUEST->GetProgramListByCategory(array(
				'CredentialToken' => $Token,
				'GetProgramListByCategoryRequestMessage' => $params
				));
				
	return $req;
}


function _affilinet_GetPropertyList($ShopId) {
	$SOAP_PROD_LOGON = new SoapClient(WSDL_DATA_LOGON);
	$ProdToken      = $SOAP_PROD_LOGON->Logon(array(
				  'Username'  => USERNAME,
				  'Password'  => PRODPASSWORD,
				  'WebServiceType' => 'Product'
				  ));
	 
	$SOAP_PROD_REQUEST = new SoapClient(WSDL_DATA);
	$req = $SOAP_PROD_REQUEST->GetPropertyList(array(
				 'PublisherID' => USERNAME,
				 'CredentialToken' => $ProdToken,
				  'ShopId' => $ShopId
				 ));

	return $req;
}


function removeUTF8BOM($json_output) {
    if (substr($json_output, 0, 3) === "\xEF\xBB\xBF") {
        return substr($json_output, 3);
    }
    return $json_output;
}

function _affilinet_SearchProductsJSON($query) {

	$url = "http://product-api.affili.net/V3/productservice.svc/JSON/SearchProducts?publisherId=736510&Password=n7u8ueQIATdC1RXcyCbn&SortBy=Price&SortOrder=descending&PageSize=25&WithImageOnly=true&ImageScales=Image120&Query=" . $_GET['q'];
	$json = file_get_contents($url);
	$json = removeUTF8BOM($json);

	$json_output = json_decode($json, TRUE);

	return $json_output;
}

function _affilinet_SearchProducts($query, $sortBy, $sortOrder, $minPrice=0.0, $maxPrice=0.0, $pageSize=10, $currentPage=1) {

	
	
	$SOAP_PROD_LOGON = new SoapClient(WSDL_DATA_LOGON);
	$ProdToken = $SOAP_PROD_LOGON->Logon(array(
				  'Username'  => USERNAME,
				  'Password'  => PRODPASSWORD,
				  'WebServiceType' => 'Product'
				  ));
	 
	$SOAP_PROD_REQUEST = new SoapClient(WSDL_DATA);
	$req = $SOAP_PROD_REQUEST->SearchProducts(array(
					'PublisherId' => USERNAME,
					'CredentialToken' => $ProdToken,
					'ShopIds' => array('0'),
					'Query'  => $query,
					'WithImageOnly' => true,
					'ImageScales' => array('Image120'),
					'LogoScales' => array('Logo120'),
					'PageSettings' => array('CurrentPage' => $currentPage,'PageSize' => $pageSize),
					'MinimumPrice' => $minPrice,
					'MaximumPrice' => $maxPrice,
					'SortBy' => $sortBy,
					'SortOrder' => $sortOrder
				 ));
	return $req;
}
